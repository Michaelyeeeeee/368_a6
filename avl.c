#include "avl.h"

int height(Node *n) { return n ? n->height : -1; }

/* Update height using child stored heights (O(1)) */
void update_height(Node *n)
{
    if (!n)
        return;
    int lh = height(n->left);
    int rh = height(n->right);
    n->height = (lh > rh ? lh : rh) + 1;
}

/* Public wrapper kept: get_height previously recursive — now O(1) */
int get_height(Node *node) { return height(node); }

/* --- rotations: update heights in O(1) after rotation --- */
Node *rotate_left(Node *node)
{
    Node *new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;

    /* update heights bottom-up */
    update_height(node);
    update_height(new_root);
    return new_root;
}

Node *rotate_right(Node *node)
{
    Node *new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;

    update_height(node);
    update_height(new_root);
    return new_root;
}

Node *rotate_left_right(Node *node)
{
    node->left = rotate_left(node->left);
    return rotate_right(node);
}

Node *rotate_right_left(Node *node)
{
    node->right = rotate_right(node->right);
    return rotate_left(node);
}

/* Balances node based on stored child heights. O(1) checks and constant rotations. */
Node *balance(Node *node)
{
    if (!node)
        return NULL;
    int lh = height(node->left);
    int rh = height(node->right);
    int bf = lh - rh;

    if (bf < -1) /* right heavy */
    {
        int rlh = height(node->right->left);
        int rrh = height(node->right->right);
        if (rlh > rrh)
            return rotate_right_left(node);
        else
            return rotate_left(node);
    }
    else if (bf > 1) /* left heavy */
    {
        int llh = height(node->left->left);
        int lrh = height(node->left->right);
        if (llh >= lrh)
            return rotate_right(node);
        else
            return rotate_left_right(node);
    }
    return node;
}

/* Recursive AVL insertion: O(log n) per insert and updates heights on the way back. */
Node *insert_rec(Node *root, Node *node)
{
    if (!root)
        return node;

    if (node->x < root->x)
        root->left = insert_rec(root->left, node);
    else
        root->right = insert_rec(root->right, node);

    update_height(root);
    return balance(root);
}

/* Public API: preserve add_node signature but call efficient insert */
void add_node(AVL *avl, Node *node)
{
    if (!avl || !node)
        return;
    avl->root = insert_rec(avl->root, node);
}

/* Simple node allocator (unchanged) */
Node *create_node(int x, int y)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        fprintf(stderr, "Error creating node: malloc\n");
        return NULL;
    }
    node->x = x;
    node->y = y;
    node->height = 0; /* leaf initially */
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* free tree */
void free_tree(Node *node)
{
    if (!node)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

/* --- build AVL by inserting from file (fast enough) --- */
AVL *create_avl(char *filename)
{
    AVL *avl = (AVL *)malloc(sizeof(AVL));
    if (!avl)
    {
        fprintf(stderr, "Error creating AVL: malloc\n");
        return NULL;
    }
    avl->root = NULL;

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        free(avl);
        return NULL;
    }
    int x, y;
    while (fscanf(file, "%d %d", &x, &y) == 2)
    {
        Node *node = create_node(x, y);
        if (!node)
            break;
        avl->root = insert_rec(avl->root, node);
    }

    fclose(file);
    return avl;
}

Node *build_balanced_from_sorted(Node **arr, int l, int r)
{
    if (l > r)
        return NULL;
    int m = (l + r) / 2;
    Node *root = arr[m];
    root->left = build_balanced_from_sorted(arr, l, m - 1);
    root->right = build_balanced_from_sorted(arr, m + 1, r);
    update_height(root);
    return root;
}
