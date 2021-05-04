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
        }else if (arg[i] == 'C') {
            data->opts.C = true;
        }else if (arg[i] == 'F') {
            data->opts.F = true;
        }else if (arg[i] == 'c') {
            data->opts.c = true;
        }else if (arg[i] == 'd') {
            data->opts.d = true;
        }else if (arg[i] == 'i') {
            data->opts.i = true;
        }else if (arg[i] == 'u') {
            data->opts.u = true;
        }else if (arg[i] == 'q') {
            data->opts.q = true;
        }else if (arg[i] == '1') {
            data->opts.one = true;
        }
        ++i;
    }
}

void parsing(t_data *data, char **argv) {
    int i = 1;
    bool on = true;

    while(argv[i]) {
        if (isOption(argv[i]) && on) {
            add_option(data, argv[i]);
        } else {
            on = false;
            add_target(data, argv[i]);
        }
        ++i;
    }
}