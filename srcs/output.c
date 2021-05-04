#include "./../ft_ls.h"

static void output_type(int type) {
    if (type == DT_DIR) { // DIRECTORY
        printf("d");
    } else if (type == DT_CHR) { // CHARACTER DEVICE
        printf("c");
    } else if (type == DT_BLK) { // BLOCK DEVICE
        printf("b"); // ??
    } else if (type == DT_FIFO) { // NAMED PIPE
        printf("p");
    } else if (type == DT_LNK) { // SYMBOLIC LINK
        printf("l");
    } else if (type == DT_REG) { // FILE
        printf("-");
    } else if (type == DT_SOCK) { // UNIX DOMAIN SOCKET
        printf("s");
    } else if (type == DT_UNKNOWN) { // UNKNOWN
        printf("u"); // ??
    }
}

static void output_permission(int p) {
    char  buff[5] = "xwr-";

    for (int i = 8; i >= 0; --i) {
        int tmp = (p & ( 1 << i )) >> i;
        if (tmp == 0) {
            printf("%c", buff[3]);
        } else {
            printf("%c", buff[i % 3]);
        }
    }
}

static void output_file(t_file *file) {
    output_type(file->type);
    output_permission(file->permissions);
    printf(" %s ", file->name);

    int i = 0;
    while(file->childs && file->childs[i]) {
        output_file(file->childs[i]);
        ++i;
    }
}

void output(t_data *data) {
    int i = 0;

    printf("childs:");
    while(data->files && data->files[i]) {
        printf("ouput[i]: %d", i);
        output_file(data->files[i]);
        printf("\n");
        ++i;
    }
}