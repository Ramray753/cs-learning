#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char **argv)
{
    struct stat mystat;
    char *type, *readok;
    stat(argv[1], &mystat);
    if (S_ISREG(mystat.st_mode)) /* Determine file type */
        type = "regular";
    else if (S_ISDIR(mystat.st_mode))
        type = "directory";
    else
        type = "other";
    if ((mystat.st_mode & S_IRUSR)) /* Check read access */
        readok = "yes";
    else
        readok = "no";
    printf("type: %s, read: %s\n", type, readok);
} 