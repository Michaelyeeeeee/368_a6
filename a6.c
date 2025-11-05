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
    int x;
    int y;
    int r;
    int collisions;
    char line[256];
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == '\n')
            break; // stop on blank line
        if (sscanf(line, "%d %d %d", &x, &y, &r) == 3)
        {
            collisions = getNumCollisions(avl->root, x, y, r);
            printf("%d\n", collisions);
        }
        else
        {
            break;
        }
    }
    free_tree(avl->root);
    free(avl);
    return 0;
}