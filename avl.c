#include "avl.h"
#include <limits.h>

/* helper min/max */
static inline int llmin(int a, int b) { return a < b ? a : b; }
static inline int llmax(int a, int b) { return a > b ? a : b; }

/* Recompute subtree_count, bbox and height from children + self.
 * Call this after any child change or rotation.
 */
void update_node(Node *n)
{
    if (!n)
        return;

    n->subtree_count = 1;
    n->min_x = n->max_x = n->x;
    n->min_y = n->max_y = n->y;

    int lh = -1, rh = -1;
    if (n->left)
    {
        n->subtree_count += n->left->subtree_count;
        n->min_x = llmin(n->min_x, n->left->min_x);
        n->max_x = llmax(n->max_x, n->left->max_x);
        n->min_y = llmin(n->min_y, n->left->min_y);
        n->max_y = llmax(n->max_y, n->left->max_y);
        lh = n->left->height;
    }
    if (n->right)
    {
        n->subtree_count += n->right->subtree_count;
        n->min_x = llmin(n->min_x, n->right->min_x);
        n->max_x = llmax(n->max_x, n->right->max_x);
        n->min_y = llmin(n->min_y, n->right->min_y);
        n->max_y = llmax(n->max_y, n->right->max_y);
        rh = n->right->height;
    }

    n->height = (lh > rh ? lh : rh) + 1;
}

/* @brief Performs a left rotation on the given node.
 *
 * @param node The node to perform the left rotation on.
 * @return The new root of the subtree after rotation.
 */
Node *rotate_left(Node *node)
{
    Node *new_root = node->right;

    node->right = new_root->left;
    new_root->left = node;

    /* update bottom-up: updated old node first, then new_root */
    update_node(node);
    update_node(new_root);

    return new_root;
}

/* @brief Performs a right rotation on the given node.
 *
 * @param node The node to perform the right rotation on.
 * @return The new root of the subtree after rotation.
 */
Node *rotate_right(Node *node)
{
    Node *new_root = node->left;

    node->left = new_root->right;
    new_root->right = node;

    /* update bottom-up */
    update_node(node);
    update_node(new_root);

    return new_root;
}

/* @brief Performs a left-right rotation on the given node.
 *
 * @param node The node to perform the left-right rotation on.
 * @return The new root of the subtree after rotation.
 */
Node *rotate_left_right(Node *node)
{
    node->left = rotate_left(node->left);
    node = rotate_right(node);

    return node;
}

/* @brief Performs a right-left rotation on the given node.
 *
 * @param node The node to perform the right-left rotation on.
 * @return The new root of the subtree after rotation.
 */
Node *rotate_right_left(Node *node)
{
    node->right = rotate_right(node->right);
    node = rotate_left(node);

    return node;
}

/* @brief Balances the given node if it is unbalanced.
 *
 * @param node The node to balance.
 * @return The new root of the subtree after balancing.
 */
Node *balance(Node *node)
{
    if (!node)
        return NULL;

    int lh = node->left ? node->left->height : -1;
    int rh = node->right ? node->right->height : -1;

    if (lh - rh < -1)
    {
        int rlh = node->right && node->right->left ? node->right->left->height : -1;
        int rrh = node->right && node->right->right ? node->right->right->height : -1;
        if (rlh > rrh)
            return rotate_right_left(node);
        else
            return rotate_left(node);
    }
    else if (lh - rh > 1)
    {
        int llh = node->left && node->left->left ? node->left->left->height : -1;
        int lrh = node->left && node->left->right ? node->left->right->height : -1;
        if (llh >= lrh)
            return rotate_right(node);
        else
            return rotate_left_right(node);
    }

    return node;
}

/* @brief Finds and balances the given node and its children.
 *
 * This version updates node metadata before deciding rotations.
 */
Node *find_and_balance(Node *node)
{
    if (!node)
        return NULL;

    node->left = find_and_balance(node->left);
    node->right = find_and_balance(node->right);

    /* recompute metadata (height, subtree_count, bounding box) */
    update_node(node);

    /* only do rotations if necessary */
    int lh = node->left ? node->left->height : -1;
    int rh = node->right ? node->right->height : -1;
    if (lh - rh == 2 || lh - rh == -2)
        node = balance(node);

    /* if rotated, rotation functions already updated metadata */
    return node;
}

/* @brief Gets the height of the given node.
 *
 * Now cheap because height is stored.
 */
int get_height(Node *node)
{
    return node ? node->height : -1;
}

/* @brief Adds a node to the AVL tree.
 *
 * @param avl The AVL tree to add the node to.
 * @param node The node to add.
 */
void add_node(AVL *avl, Node *node)
{
    if (!avl->root)
    {
        avl->root = node;
        return;
    }

    Node *root = avl->root;
    Node *pre = NULL;
    while (root)
    {
        pre = root;
        if (node->x < root->x)
            root = root->left;
        else // node->x >= root->x
            root = root->right;
    }
    if (node->x < pre->x)
        pre->left = node;
    else
        pre->right = node;

    avl->root = find_and_balance(avl->root);
}

/* @brief Creates a new node with the given x and y coordinates.
 *
 * @param x The x coordinate of the node.
 * @param y The y coordinate of the node.
 * @return The newly created node.
 */
Node *create_node(int x, int y)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        printf("Error creating node: malloc\n");
        return NULL;
    }
    node->x = x;
    node->y = y;
    node->height = 0;
    node->subtree_count = 1;
    node->min_x = node->max_x = x;
    node->min_y = node->max_y = y;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* @brief Frees the memory allocated for the AVL tree.
 *
 * @param node The root node of the AVL tree.
 */
void free_tree(Node *node)
{
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

/* @brief Creates an AVL tree from the given file.
 *
 * @param filename The name of the file to read the nodes from.
 * @return The newly created AVL tree.
 */
AVL *create_avl(char *filename)
{
    AVL *avl = (AVL *)malloc(sizeof(AVL));
    if (!avl)
    {
        printf("Error creating AVL: malloc\n");
    }
    avl->root = NULL;

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file: %s\n", filename);
        free(avl);
        return NULL;
    }
    int x;
    int y;
    while (fscanf(file, "%d %d", &x, &y) == 2)
    {
        Node *node = create_node(x, y);
        add_node(avl, node);
    }

    fclose(file);
    return avl;
}
