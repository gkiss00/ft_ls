#include "./../ft_ls.h"

static void output_node_number(ino_t nb) {
    printf("%15llu ", nb);
}

static void output_nb_child(t_file *f) {
    if (f->type == DT_DIR)
        printf("%5d", file_nb_child(f));
    else if (f->type == DT_REG)
        printf("%5d", 1);
}

static void output_date(time_t timestamp) {
    char *str = ctime(&timestamp);
    char **tab = ft_split(str, ' ');
    to_lower(tab[1]);
    char *t = ft_substr(tab[3], 0, 5);
    printf("%3s %s %s", tab[2], tab[1], t);

    int i = 0;
    while(tab && tab[i]){
        free(tab[i]);
        ++i;
    }
    free(tab);
    free(t);
}

static void output_size(int size) {
    printf("%12d", size);
}

static void output_team(char *group) {
    printf("%6s", group);
}

static void output_member(char *member) {
    printf("%15s", member);
}

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

static void output_file(t_file *file, t_data *data, bool recur) {
    output_node_number(file->node_id);
    output_type(file->type);
    output_permission(file->permissions);
    output_nb_child(file);
    output_member(file->owner);
    output_team(file->team);
    output_size(file->size);
    output_date(file->c_time);
    printf(" %s\n", file->name);

    if (data->opts.R || recur) {
        int i = 0;
        while(file->childs && file->childs[i]) {
            output_file(file->childs[i], data, false);
            ++i;
        }
    }
}

void output(t_data *data) {
    int i = 0;

    while(data->files && data->files[i]) {
        output_file(data->files[i], data, true);
        printf("\n");
        ++i;
    }
}