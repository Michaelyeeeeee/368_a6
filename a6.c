#include "a6.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

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
    long long x, y, r;
    int collisions;

    /* Read queries line-by-line and accept only lines that are exactly three
     * integers separated by single space characters ("<int> <int> <int>\n").
     * Any other input (empty line, wrong delimiter, non-numeric chars, too
     * many/few numbers, overflow, or overly long lines) will stop the loop.
     */
    char line[256];
    while (fgets(line, sizeof line, stdin))
    {
        size_t len = strlen(line);

        /* empty line or just a newline -> stop */
        if (len == 0 || (len == 1 && line[0] == '\n'))
            break;

        /* if the line didn't end with a newline it was too long for the
         * buffer -> treat as invalid and stop
         */
        if (line[len - 1] != '\n')
            break;

        /* strip trailing newline */
        line[len - 1] = '\0';

        /* enforce first character is a digit or a minus (no leading spaces)
         * to ensure format exactly as: "<int> <int> <int>"
         */
        if (!(line[0] == '-' || (line[0] >= '0' && line[0] <= '9')))
            break;

        char *p = line;
        char *end = NULL;
        long v1, v2, v3;
        errno = 0;

        v1 = strtol(p, &end, 10);
        if (end == p || errno == ERANGE)
            break;
        /* next char must be a single space */
        if (*end != ' ')
            break;

        p = end + 1;
        errno = 0;
        v2 = strtol(p, &end, 10);
        if (end == p || errno == ERANGE)
            break;
        if (*end != ' ')
            break;

        p = end + 1;
        errno = 0;
        v3 = strtol(p, &end, 10);
        if (end == p || errno == ERANGE)
            break;
        /* after third number we must be at string end */
        if (*end != '\0')
            break;

        /* check values fit in int */
        if (v1 < INT_MIN || v1 > INT_MAX || v2 < INT_MIN || v2 > INT_MAX || v3 < INT_MIN || v3 > INT_MAX)
            break;

        x = (long long)v1;
        y = (long long)v2;
        r = (long long)v3;

        collisions = getNumCollisions(avl->root, x, y, r);
        printf("%d\n", collisions);
    }

    free_tree(avl->root);
    free(avl);
    return 0;
}