#include "./../ft_ls.h"

static bool isOption(char *arg) {
    return (arg[0] == '-');
}

static void add_target(t_data *data, char *target) {
    node_add_back(&data->target, new_node(target));
}

static void add_option(t_data *data, char *arg) {
    int i = 1;

    while(arg[i]) {
        if (arg[i] == 'a') {
            data->opts.a = true;
        } else if (arg[i] == 'l') {
            data->opts.l = true;
        } else if (arg[i] == 'r') {
            data->opts.r = true;
        } else if (arg[i] == 'R') {
            data->opts.R = true;
        } else if (arg[i] == 't') {
            data->opts.t = true;
        }
        ++i;
    }
}

void parsing(t_data *data, char **argv) {
    int i = 1;
    bool on = true;

    while(argv[i]) {
        printf("arg: %s\n", argv[i]);
        if (isOption(argv[i]) && on) {
            add_option(data, argv[i]);
        } else {
            on = false;
            add_target(data, argv[i]);
        }
        ++i;
    }
}