#include "collision.h"
#include "avl.h"

/* @brief Checks if the distance between the point (x, y) and the node is less than or equal to r.
 *
 * @param x The x coordinate of the point.
 * @param y The y coordinate of the point.
 * @param r The radius.
 * @param node The node to check the distance to.
 * @return 1 if the distance is less than or equal to r, 0 otherwise.
 */
int checkDistance(long long x, long long y, long long r, Node *node)
{
    int node_x = node->x;
    int node_y = node->y;
    long long dx = (long long)x - (long long)node_x;
    long long dy = (long long)y - (long long)node_y;
    long long dist_sq = dx * dx + dy * dy;
    long long radius_sq = (long long)r * r;

    return dist_sq <= radius_sq;
}

/* @brief Gets the number of collisions (nodes within radius r) in the AVL tree.
 *
 * @param node The root node of the AVL tree.
 * @param x The x coordinate of the point.
 * @param y The y coordinate of the point.
 * @param r The radius.
 * @return The number of collisions.
 */
int getNumCollisions(Node *node, long long x, long long y, long long r)
{
    if (!node)
        return 0;

    int count = 0;
    if (checkDistance(x, y, r, node))
        count++;

    // Check both subtrees as points within radius r could be in either subtree
    if ((x - r) <= node->x)
        count += getNumCollisions(node->left, x, y, r);
    if ((x + r) >= node->x)
        count += getNumCollisions(node->right, x, y, r);

    return count;
}