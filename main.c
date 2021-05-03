#include "./ft_ls.h"

t_data data;

static void read_permission(struct stat st) {
    for (int i = 0; i < 9; ++i) {
        printf("mode %d: %d\n", i, (st.st_mode & ( 1 << i )) >> i);
    }
}

static void read_dir(t_node *node) {
    DIR *target = opendir(node->target);

    if (target) {
        struct dirent *res = readdir(target);
        while((res = readdir(target)) != NULL) {
            if (res) {
                printf("name: %s\ntype: %d\nsize: %d\nino: %llu\nseekoff: %llu\n\n", res->d_name, res->d_type, res->d_reclen, res->d_ino, res->d_seekoff);
                struct stat stat_res;
                stat(node->target, &stat_res);
                printf("size: %lld\n", stat_res.st_size);
                read_permission(stat_res);
                //printf("mode: %d\n", stat_res.st_mode) ;
                printf("a_time: %ld\n", stat_res.st_atimespec.tv_sec);
                printf("c_time: %ld\n", stat_res.st_ctimespec.tv_sec);
                printf("b_time: %ld\n", stat_res.st_birthtimespec.tv_sec);
                printf("%s\n", ctime(&stat_res.st_birthtimespec.tv_sec));
                struct group *grp = getgrgid(stat_res.st_gid);
                printf("group name: %s\n", grp->gr_name);
                printf("group pwd: %s\n", grp->gr_passwd);
                printf("group id: %u\n", grp->gr_gid);
                int x = 0;
                while(grp->gr_mem[x]) {
                    printf("group member: %s\n", grp->gr_mem[x]);
                    ++x;
                }
                printf("\n*****************************\n\n");
            }
            // char buff[200];
            // listxattr(node->target, buff, 200, XATTR_SHOWCOMPRESSION);
            // printf("xxx: %d", printf("buff : %s\n\n\n\n\", buff));
        }
    }
    closedir(target);
}

int main(int argc, char **argv) {
    argc = 0;
    init(&data);
    parsing(&data, argv);

    t_node *tmp = data.target;
    while(tmp) {
        printf("%s =>\n", tmp->target);
        read_dir(tmp);
        tmp = tmp->next;
    }
    free_node(data.target);
}