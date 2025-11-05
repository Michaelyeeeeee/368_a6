#include "a6.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    char *filename = argv[1];
    AVL *avl = create_avl(filename);
    if (!avl)
    {
        return 1;
    }
    int x, y, r;
    int collisions;
    while (scanf("%d %d %d", &x, &y, &r) == 3)
    {
        collisions = getNumCollisions(avl->root, x, y, r);
        printf("%d\n", collisions);
    }

    free_tree(avl->root);
    free(avl);
    return 0;
}