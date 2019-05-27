#include "header.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        prim("in.txt", "out.txt");
    } else if (argc == 3) {
        prim(argv[1], argv[2]);
    } else {
        print_help();
        return 1;
    }
    return 0;
}