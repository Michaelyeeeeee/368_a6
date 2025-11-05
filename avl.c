#include "avl.h"

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

    return new_root;
};

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
    /* Use safe child height values. */
    int lh = node->left ? node->left->height : -1;
    int rh = node->right ? node->right->height : -1;

    if (lh - rh < -1)
    {
        /* Right heavy */
        int rlh = node->right->left ? node->right->left->height : -1;
        int rrh = node->right->right ? node->right->right->height : -1;
        if (rlh > rrh)
            return rotate_right_left(node);
        else
            return rotate_left(node);
    }
    else if (lh - rh > 1)
    {
        /* Left heavy */
        int llh = node->left->left ? node->left->left->height : -1;
        int lrh = node->left->right ? node->left->right->height : -1;
        if (llh >= lrh)
            return rotate_right(node);
        else
            return rotate_left_right(node);
    }

    return node;
}

/* @brief Finds and balances the given node and its children.
 *
 * @param node The node to find and balance.
 * @return The new root of the subtree after balancing.
 */
Node *find_and_balance(Node *node)
{
    if (!node)
        return NULL;

    node->left = find_and_balance(node->left);
    node->right = find_and_balance(node->right);
    node->height = get_height(node);

    /* Compute child heights safely (children may be NULL). */
    int lh = node->left ? node->left->height : -1;
    int rh = node->right ? node->right->height : -1;

    /* only balance if lowest unbalanced node */
    if (lh - rh == 2 || lh - rh == -2)
        node = balance(node);
    return node;
}

/* @brief Gets the height of the given node.
 *
 * @param node The node to get the height of.
 * @return The height of the node.
 */
int get_height(Node *node)
{
    /* Return -1 for NULL so that an empty child has height -1, leaf has 0. */
    if (!node)
        return -1;
    int left = get_height(node->left);
    int right = get_height(node->right);

    return ((left > right) ? left : right) + 1;
}

/* @brief Adds a node to the AVL tree.
 *
 * @param avl The AVL tree to add the node to.
 * @param node The node to add.
 */
void add_node(AVL *avl, Node *node)
{
    /* If tree is empty, set root to new node. */
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