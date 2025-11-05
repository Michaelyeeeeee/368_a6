#include "collision.h"
#include "avl.h"
#include <stdlib.h>
#include <math.h>

/* inline helpers for rectangle-circle tests */

/* Closest point on rectangle to (cx,cy) then squared distance */
static inline unsigned long long rect_dist_sq_to_point(long long min_x, long long max_x,
                                                       long long min_y, long long max_y,
                                                       long long cx, long long cy)
{
    long long px = cx;
    if (cx < min_x)
        px = min_x;
    else if (cx > max_x)
        px = max_x;

    long long py = cy;
    if (cy < min_y)
        py = min_y;
    else if (cy > max_y)
        py = max_y;

    long long dx = px - cx;
    long long dy = py - cy;
    return (unsigned long long)dx * dx + (unsigned long long)dy * dy;
}

/* Max distance (squared) from (cx,cy) to any rectangle corner */
static inline unsigned long long rect_max_corner_dist_sq(long long min_x, long long max_x,
                                                         long long min_y, long long max_y,
                                                         long long cx, long long cy)
{
    long long dx = llabs(min_x - cx);
    long long dx2 = llabs(max_x - cx);
    long long maxdx = dx > dx2 ? dx : dx2;
    long long dy = llabs(min_y - cy);
    long long dy2 = llabs(max_y - cy);
    long long maxdy = dy > dy2 ? dy : dy2;
    return (unsigned long long)maxdx * maxdx + (unsigned long long)maxdy * maxdy;
}

/* If rect is completely outside circle -> no intersection */
static inline int rect_outside_circle(Node *n, long long cx, long long cy, long long r)
{
    unsigned long long closest_sq = rect_dist_sq_to_point(n->min_x, n->max_x, n->min_y, n->max_y, cx, cy);
    unsigned long long r_sq = (unsigned long long)r * r;
    return closest_sq > r_sq;
}

/* If rect entirely inside circle */
static inline int rect_inside_circle(Node *n, long long cx, long long cy, long long r)
{
    unsigned long long maxcorner_sq = rect_max_corner_dist_sq(n->min_x, n->max_x, n->min_y, n->max_y, cx, cy);
    unsigned long long r_sq = (unsigned long long)r * r;
    return maxcorner_sq <= r_sq;
}

/* old checkDistance kept for compatibility (not strictly needed) */
int checkDistance(long long dx, long long dy, long long r, Node *node)
{
    unsigned long long dist_sq = (unsigned long long)dx * dx + (unsigned long long)dy * dy;
    unsigned long long radius_sq = (unsigned long long)r * r;
    return dist_sq <= radius_sq;
}

/* getNumCollisions with pruning using bbox and subtree_count */
int getNumCollisions(Node *node, long long cx, long long cy, long long r)
{
    if (!node)
        return 0;

    /* whole subtree outside circle? */
    if (rect_outside_circle(node, cx, cy, r))
        return 0;

    /* whole subtree inside circle? return count without traversing */
    if (rect_inside_circle(node, cx, cy, r))
        return node->subtree_count;

    /* partial overlap: check this node and recurse */
    int count = 0;
    long long dx = node->x - cx;
    long long dy = node->y - cy;
    if (checkDistance(dx, dy, r, node))
        count++;

    count += getNumCollisions(node->left, cx, cy, r);
    count += getNumCollisions(node->right, cx, cy, r);

    return count;
}
