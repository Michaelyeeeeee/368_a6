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
int checkDistance(int x, int y, int r, Node *node)
{
    int node_x = node->x;
    int node_y = node->y;
    int dist_sq = (x - node_x) * (x - node_x) + (y - node_y) * (y - node_y);
    int radius_sq = r * r;

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
int getNumCollisions(Node *node, int x, int y, int r)
{
    if (!node)
        return 0;

    int count = 0;
    if (checkDistance(x, y, r, node))
        count++;

    // Don't check all nodes, only those within x ± r
    if (node->x >= x - r)
        count += getNumCollisions(node->left, x, y, r);
    if (node->x <= x + r)
        count += getNumCollisions(node->right, x, y, r);

    return count;
}