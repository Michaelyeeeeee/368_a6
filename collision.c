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
    return pow(pow(x - node->x, 2) + pow(y - node->y, 2), 0.5) <= pow(r, 2);
}

/* @brief Gets the maximum and minimum x and y coordinates for a square centered at (x, y) with side length 2r.
 *
 * @param x The x coordinate of the center point.
 * @param y The y coordinate of the center point.
 * @param r The radius.
 * @param maxX Pointer to store the maximum x coordinate.
 * @param minX Pointer to store the minimum x coordinate.
 * @param maxY Pointer to store the maximum y coordinate.
 * @param minY Pointer to store the minimum y coordinate.
 */
void getMaxMinXMaxMinY(int x, int y, int r, int *maxX, int *minX, int *maxY, int *minY)
{
    *maxX = x + r;
    *minX = x - r;
    *maxY = y + r;
    *minY = y - r;
}
/* @brief Gets the number of collisions (nodes within radius r) in the AVL tree.
 *
 * @param node The root node of the AVL tree.
 * @param x The x coordinate of the point.
 * @param y The y coordinate of the point.
 * @param r The radius.
 * @return The number of collisions.
 */
int getNumCollisions(Node *node, int maxX, int minX, int maxY, int minY, int r)
{
    if (!node)
        return 0;

    int count = 0;

    // check if within box of side length r*2 centered at (x, y)
    if (node->x <= maxX && node->x >= minX && node->y <= maxY && node->y >= minY)
    {
        if (checkDistance(node->x, node->y, r, node))
            count++;
    }

    // get collisions until leftmost/rightmost possible nodes
    //  go left until out of range
    if (minX <= node->x)
        count += getNumCollisions(node->left, maxX, minX, maxY, minY, r);
    // go right until out of range
    if (maxX >= node->x)
        count += getNumCollisions(node->right, maxX, minX, maxY, minY, r);
    return count;
}