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
    return pow((long long)pow(x - node->x, 2) + (long long)pow(y - node->y, 2), 0.5) <= (long long)pow(r, 2);
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

    // check if within box of side length r*2 centered at (x, y)
    int dx = (int)node->x - x;
    int dy = (int)node->y - y;
    if (dx <= r && dx >= -1 * r && dy <= r && dy >= -1 * r)
    {
        if (checkDistance(x, y, r, node))
            count++;
    }

    // get collisions until leftmost/rightmost possible nodes
    //  go left until out of range
    if ((x - r) <= node->x)
        count += getNumCollisions(node->left, x, y, r);
    // go right until out of range
    if ((x + r) >= node->x)
        count += getNumCollisions(node->right, x, y, r);
    return count;
}