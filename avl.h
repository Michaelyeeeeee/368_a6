#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int x;
    int y;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct AVL
{
    struct Node *root;
} AVL;

Node *rotate_left(Node *node);
Node *rotate_right(Node *node);
Node *rotate_left_right(Node *node);
Node *rotate_right_left(Node *node);
Node *balance(Node *node);
Node *find_and_balance(Node *node);
int get_height(Node *node);
AVL *create_avl(char *filename);

void add_node(AVL *avl, Node *node);
Node *create_node(int x, int y);
void free_tree(Node *node);