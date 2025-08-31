/* Minimal isfloppy implementation for standalone build */
#include <lib/libsa/stand.h>

extern char *strstr(const char *, const char *);

int
bootdev_isfloppy(const char *bootdev)
{
    if (bootdev == NULL)
        return 0;
    /* Heuristic: consider floppy if boot path contains ":fd" or "/floppy" */
    if (strstr(bootdev, ":fd") || strstr(bootdev, "/floppy"))
        return 1;
    return 0;
}


