#include "./../ft_ls.h"

void    init(t_data *data) {
    data->opts.a = false;
    data->opts.l = false;
    data->opts.r = false;
    data->opts.R = false;
    data->opts.t = false;
    data->opts.C = false;
    data->opts.F = false;
    data->opts.c = false;
    data->opts.d = false;
    data->opts.i = false;
    data->opts.u = false;
    data->opts.q = false;
    data->opts.one = false;
    data->target = NULL;
}