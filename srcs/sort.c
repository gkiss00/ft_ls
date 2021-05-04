#include "./../ft_ls.h"

//
// REVERSE
//

static void reverse_file(t_file *file) {
    int i = 0;
    int size = file_nb_child(file);

    while(file->childs && file->childs[i]) {
        if (file->childs[i]->type == DT_DIR)
            reverse_file(file->childs[i]);
        ++i;
    }
    i = 0;
    while(i < size / 2) {
        t_file *tmp = file->childs[i];
        file->childs[i] = file->childs[size - i - 1];
        file->childs[size - i - 1] = tmp;
        ++i;
    }
}

static void reverse(t_data *data) {
    int i = 0;
    int size = file_nb_child2(data->files);

    while(i < size / 2) {
        t_file *tmp = data->files[i];
        data->files[i] = data->files[size - i - 1];
        data->files[size - i - 1] = tmp;
        ++i;
    }
    i = 0;
    while(data->files && data->files[i]) {
        if (data->files[i]->type == DT_DIR)
            reverse_file(data->files[i]);
        ++i;
    }
}

//
// SORT DATE BY READING NAME
//

static void sort_file_by_name(t_file *file) {
    int i = 0;

    while(file->childs && file->childs[i]) {
        if (file->childs[i]->type == DT_DIR)
            sort_file_by_name(file->childs[i]);
        ++i;
    }
    i = 0;
    while(file->childs && file->childs[i] && file->childs[i + 1]) {
        //printf("%s %s %d\n", file->childs[i]->name, file->childs[i + 1]->name, strcmp(file->childs[i]->name, file->childs[i + 1]->name));
        if (strcmp(file->childs[i]->name, file->childs[i + 1]->name) > 0) {
            t_file *tmp = file->childs[i];
            file->childs[i] = file->childs[i + 1];
            file->childs[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
}

static void sort_by_name(t_data *data) {
    int i = 0;

    while(data->files && data->files[i] && data->files[i + 1]) {
        //printf("%s %s %d\n", data->files[i]->name, data->files[i + 1]->name, strcmp(data->files[i]->name, data->files[i + 1]->name));
        if (strcmp(data->files[i]->name, data->files[i + 1]->name) > 0) {
            t_file *tmp = data->files[i];
            data->files[i] = data->files[i + 1];
            data->files[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
    i = 0;
    while(data->files && data->files[i]) {
        if (data->files[i]->type == DT_DIR)
            sort_file_by_name(data->files[i]);
        ++i;
    }
}

//
// SORT DATE BY CREATION DATE
//

static void sort_file_c_date(t_file *file) {
    int i = 0;

    while(file->childs && file->childs[i]) {
        if (file->childs[i]->type == DT_DIR)
            sort_file_c_date(file->childs[i]);
        ++i;
    }
    i = 0;
    while(file->childs && file->childs[i] && file->childs[i + 1]) {
        if (file->childs[i]->c_time > file->childs[i + 1]->c_time) {
            t_file *tmp = file->childs[i];
            file->childs[i] = file->childs[i + 1];
            file->childs[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
}

static void sort_by_c_date(t_data *data) {
    int i = 0;
    while(data->files && data->files[i] && data->files[i + 1]) {
        if (data->files[i]->c_time > data->files[i + 1]->c_time) {
            t_file *tmp = data->files[i];
            data->files[i] = data->files[i + 1];
            data->files[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
    i = 0;
    while(data->files && data->files[i]) {
        if (data->files[i]->type == DT_DIR)
            sort_file_c_date(data->files[i]);
        ++i;
    }
}

//
// SORT DATE BY UPDATE DATE
//

static void sort_file_u_date(t_file *file) {
    int i = 0;

    while(file->childs && file->childs[i]) {
        if (file->childs[i]->type == DT_DIR)
            sort_file_u_date(file->childs[i]);
        ++i;
    }
    i = 0;
    while(file->childs && file->childs[i] && file->childs[i + 1]) {
        if (file->childs[i]->u_time > file->childs[i + 1]->u_time) {
            t_file *tmp = file->childs[i];
            file->childs[i] = file->childs[i + 1];
            file->childs[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
}

static void sort_by_u_date(t_data *data) {
    int i = 0;
    while(data->files && data->files[i] && data->files[i + 1]) {
        if (data->files[i]->u_time > data->files[i + 1]->u_time) {
            t_file *tmp = data->files[i];
            data->files[i] = data->files[i + 1];
            data->files[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
    i = 0;
    while(data->files && data->files[i]) {
        if (data->files[i]->type == DT_DIR)
            sort_file_u_date(data->files[i]);
        ++i;
    }
}

//
// SORT DATE BY READING DATE
//

static void sort_file_r_date(t_file *file) {
    int i = 0;

    while(file->childs && file->childs[i]) {
        if (file->childs[i]->type == DT_DIR)
            sort_file_r_date(file->childs[i]);
        ++i;
    }
    i = 0;
    while(file->childs && file->childs[i] && file->childs[i + 1]) {
        if (file->childs[i]->r_time > file->childs[i + 1]->r_time) {
            t_file *tmp = file->childs[i];
            file->childs[i] = file->childs[i + 1];
            file->childs[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
}

static void sort_by_r_date(t_data *data) {
    int i = 0;
    while(data->files && data->files[i] && data->files[i + 1]) {
        if (data->files[i]->r_time > data->files[i + 1]->r_time) {
            t_file *tmp = data->files[i];
            data->files[i] = data->files[i + 1];
            data->files[i + 1] = tmp;
            i = -1;
        }
        ++i;
    }
    i = 0;
    while(data->files && data->files[i]) {
        if (data->files[i]->type == DT_DIR)
            sort_file_r_date(data->files[i]);
        ++i;
    }
}

void sort(t_data *data) {
    if (data->opts.c) {
        sort_by_r_date(data);
    } else if (data->opts.u) {
        sort_by_u_date(data);
    } else if (data->opts.t) {
        sort_by_c_date(data);
    } else {
        sort_by_name(data);
    }
    if (data->opts.r) {
        reverse(data);
    }
}