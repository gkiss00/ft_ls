#include "./ft_ls.h"

t_data data;

static void add_file(t_file *parent, struct dirent *res, int i) {
    t_file *new_file = new_empty_file();
    struct stat stat_res;
    struct group *grp;
    stat(res->d_name, &stat_res);
    grp = getgrgid(stat_res.st_gid);

    new_file->name = strdup(res->d_name);
    new_file->type = res->d_type;

    new_file->nb_link = 0;

    new_file->size = stat_res.st_size;
    new_file->permissions = stat_res.st_mode;
    new_file->c_time = stat_res.st_birthtimespec.tv_sec;
    new_file->u_time = stat_res.st_atimespec.tv_sec;
    new_file->r_time = stat_res.st_ctimespec.tv_sec;

    new_file->team = strdup(grp->gr_name);
    int k = 0;
    while(grp->gr_mem[k]) {
        free(new_file->owner);
        new_file->owner = strdup(grp->gr_mem[k]);
        ++k;
    }
    new_file->accessibility = true;
    new_file->childs = NULL;
    if(i == 0){
        add_root(&data, new_file);
    } else {
        file_add_child(parent, new_file);
    }
}

static void read_dir(char *path, int x) {
    DIR *target = opendir(path);
    int i = 0;

    if (target) {
        struct dirent *res;
        while((res = readdir(target)) != NULL) {
            if (res) {
                printf("i: %d\n", i);
                if(i == 0)
                    add_file(NULL, res, i);
                else
                    add_file(data.files[x], res, i);
            }
            ++i;
        }
    }
    closedir(target);
}

int main(int argc, char **argv) {
    argc = 0;
    init(&data);
    parsing(&data, argv);

    t_node *tmp = data.target;
    int x = 0;
    while(tmp) {
        printf("before: %d\n", x);
        read_dir(tmp->target, x);
        tmp = tmp->next;
        printf("after: %d\n", x);
        ++x;
    }
    printf("output\n");
    // sort(&data);
    // output(&data);
    free_data(&data);
}