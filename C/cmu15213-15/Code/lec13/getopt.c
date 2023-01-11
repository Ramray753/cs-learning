#include <stdio.h>
#include <unistd.h>

int main (int argc, char **argv){
    int opt;
    while ((opt = getopt (argc, argv, ":abc:d")) != -1){
        if (opt == '?'){
            printf("unknown flag: %c\n", optopt);
        } else if (opt == ':'){
            printf("flag: %c, value not given\n", optopt);
        }
        else {
            printf("flag: %c, value: %s\n", opt, optarg);
        }
    }
    for (int i = optind; i < argc; i++) {
        printf("remaining: %s\n", argv[i]);
    }
}