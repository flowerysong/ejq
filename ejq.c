#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <json-selector.h>
#include <json.h>

char *
read_file(const char *path) {
    char *buffer;
    int   fd = open(path, O_RDONLY, 0666);
    if (fd == -1) {
        fprintf(stderr, "Cannot access file %s", path);
        exit(1);
    }

    long size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    buffer = calloc(sizeof(char), size);

    if (read(fd, buffer, size) != size) {
        fprintf(stderr, "Failed to read file %s: %m\n", path);
        exit(1);
    }

    close(fd);
    return buffer;
}

int
main(int ac, char *av[]) {
    int   c;
    int   err = 0;
    bool  raw = false;
    char *expr = NULL;
    char *fname = NULL;

    while ((c = getopt(ac, av, "e:f:r")) != -1) {
        switch (c) {
        case 'e':
            expr = optarg;
            break;

        case 'f':
            fname = optarg;
            break;

        case 'r':
            raw = true;
            break;

        default:
            err++;
        }
    }

    if (err || optind != ac) {
        fprintf(stderr, "Usage:\t%s", av[ 0 ]);
        fprintf(stderr, " -e <expression> -f <file> [-r]\n");
        exit(1);
    }

    json *parsed = jsonParse(read_file(fname));
    json *res = jsonSelect(parsed, expr);

    if (jsonIsArray(res)) {
        res = res->array;
    } else if (jsonIsObject(res)) {
        res = res->object;
    }

    if (raw) {

    } else {
        printf("%s", jsonToString(res, NULL));
    }
}
