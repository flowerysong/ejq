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
        fprintf(stderr, "Cannot access file %s\n", path);
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


static void
raw_print(json *obj) {
    json *arr = NULL;

    if (obj == NULL) {
        return;
    }

    switch (obj->type) {
    case JSON_STRING:
        printf("%s\n", obj->str);
        break;

    case JSON_ARRAY:
        arr = obj->array;
        while (arr) {
            raw_print(arr);
            arr = arr->next;
        }
        break;

    default:
        /* just the normal repr */
        printf("%s\n", jsonToString(obj, NULL));
        break;
    }
}


int
main(int ac, char *av[]) {
    int   c;
    int   err = 0;
    bool  raw = false;
    char *expr = NULL;
    char *fname = NULL;
    json *parsed = NULL;
    json *res = NULL;

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
        fprintf(stderr, " -f <file> [-e <expression>] [-r]\n");
        exit(1);
    }

    parsed = jsonParse(read_file(fname));
    if (expr) {
        res = jsonSelect(parsed, expr);
        /* Selection just returns a pointer to the appropriate place in the
         * structure, to easily only output the selected bit we need to throw
         * away the key and the following elements (if any).
         */
        if (res) {
            res->key = NULL;
            res->next = NULL;
        }
    } else {
        res = parsed;
    }

    if (raw) {
        raw_print(res);
    } else {
        printf("%s\n", jsonToString(res, NULL));
    }
}
