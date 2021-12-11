#include "ft_ls.h"

// FLAGS
bool l = false;
bool R = false;
bool a = false;
bool r = false;
bool t = false;

void fill_opts(int argc, char **argv);
void sort(char **files);
int get_tab_size(char **tab);

void fill_opts(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "lRart")) != -1) {
        switch (opt) {
        case 'l':
            l = true;
            break;
        case 'R':
            R = true;
            break;
        case 'a':
            a = true;
            break;
        case 'r':
            r = true;
            break;
        case 't':
            t = true;
            break;
        default: /* aq?aq */
            fprintf(stderr, "Usage: %s [-lRart] path\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}

int get_tab_size(char **tab) {
    int i = 0;

    while(tab && tab[i]) {
        ++i;
    }
    return i;
}

// bubble sort
void sort(char **files) {
    int size = get_tab_size(files);

    for (int i = 0; i < size - 1; ++i) { 
        bool swapped = false;
        for (int j = 0; j < size - i - 1; ++j) {
            if (!r) {
                // reverse
                if (strcmp(files[j], files[j + 1]) > 0) {
                    char *tmp = files[j];
                    files[j] = files[j + 1];
                    files[j + 1] = tmp;
                    swapped = true;
                }
            } else {
                if (strcmp(files[j], files[j + 1]) < 0) {
                    char *tmp = files[j];
                    files[j] = files[j + 1];
                    files[j + 1] = tmp;
                    swapped = true;
                }
            }
        }
        if(!swapped)
            break;
    }
}

void output_type(unsigned char type) {
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

void output_permissions(mode_t mode) {
    // user permissions
    if ((mode & S_IRUSR) == S_IRUSR)
        printf("r");
    else
        printf("-");
    if ((mode & S_IWUSR) == S_IWUSR)
        printf("w");
    else
        printf("-");
    if ((mode & S_IXUSR) == S_IXUSR)
        printf("x");
    else
        printf("-");
    // group permissions
    if ((mode & S_IRGRP) == S_IRGRP)
        printf("r");
    else
        printf("-");
    if ((mode & S_IWGRP) == S_IWGRP)
        printf("w");
    else
        printf("-");
    if ((mode & S_IXGRP) == S_IXGRP)
        printf("x");
    else
        printf("-");
    // others permissions
    if ((mode & S_IROTH) == S_IROTH)
        printf("r");
    else
        printf("-");
    if ((mode & S_IWOTH) == S_IWOTH)
        printf("w");
    else
        printf("-");
    if ((mode & S_IXOTH) == S_IXOTH)
        printf("x");
    else
        printf("-");
}

void ouput_timestamp(time_t time) {
    char *timestamp = ctime(&time);
    char tmp[strlen(timestamp)];
    memset(tmp, 0, strlen(timestamp));
    memcpy(tmp, timestamp, strlen(timestamp) - 1);
    printf(" %s", tmp);
}

void output_user(uid_t id) {
    struct passwd *pwd;
    pwd = getpwuid(id);
    if(pwd)
        printf(" %s", pwd->pw_name);
}

void output_group(gid_t id) {
    struct group *group;
    group = getgrgid(id);
    if(group)
        printf(" %s", group->gr_name);
}

void output_name(t_file *file, char *symlink) {
    if (file->type == DT_DIR) { // DIRECTORY
        printf(BOLD_CYAN);
        //printf(CYAN);
    } else if (file->type == DT_CHR) { // CHARACTER DEVICE
        printf(BLUE_ON_YELLOW);
        //printf(RED);
    } else if (file->type == DT_BLK) { // BLOCK DEVICE
        printf(BLUE_ON_CYAN);
    } else if (file->type == DT_FIFO) { // NAMED PIPE
        //printf("p");
    } else if (file->type == DT_LNK) { // SYMBOLIC LINK
        printf(PURPLE);
    } else if (file->type == DT_REG) { // FILE
        //printf("-");
    } else if (file->type == DT_SOCK) { // UNIX DOMAIN SOCKET
        //printf("s");
    } else if (file->type == DT_UNKNOWN) { // UNKNOWN
        //printf("u"); // ??
    }
    printf(" %s", file->name);
    printf(WHITE);
    if(file->type == DT_LNK && symlink != NULL)
        printf(" -> %s", symlink);
}

void output_file(t_file *file, char *path) {
    if (a || (!a && file->name[0] != '.')) {
        if (l) {
            struct stat buff;
            char *new_path = strjoin(strjoin(path, "/", FREE_S0), file->name, FREE_S1);
            int stat_res = stat(new_path, &buff);
            char *symlink = NULL;
            if (file->type == DT_LNK){
                char c_buff[256];
                int readlink_res = readlink(new_path, c_buff, 255);
                if (readlink_res < 0) {
                    fprintf(stderr, "%s: Can not read symlink\n", new_path);
                } else {
                    symlink = calloc(readlink_res + 1, 1);
                    if(symlink == NULL) {
                        fprintf(stderr, "Malloc error\n");
                        exit(EXIT_FAILURE);
                    }
                    memcpy(symlink, c_buff, readlink_res);
                }
            }
                
            // type permissions nb_link proprio group taille eurodatage nom
            if(stat_res >= 0) {
                output_type(file->type);
                output_permissions(buff.st_mode);
                if(strcmp(file->name, "..") == 0)
                    printf("@");
                printf(" %d", (int)buff.st_nlink);
                output_user(buff.st_uid);
                output_group(buff.st_gid);
                printf(" %ld", (long)buff.st_size);
                ouput_timestamp(buff.st_ctimespec.tv_sec);
                output_name(file, symlink);
                printf("\n");
            } else {
                printf("%s: Permission denied\n", new_path);
            }
            free(new_path);
            free(symlink);
        } else {
            output_name(file, NULL);
        }
    }
}

void read_dir(char *path) {
    DIR *dir = opendir(path);
    if(dir == NULL) {
        fprintf(stderr, "ft_ls: %s: No such file or directory", path);
        return ;
    }

    t_file **files = NULL;
    struct dirent *res = NULL;
    while((res = readdir(dir)) != NULL) {
        files = add_file_to_tab_of_file(files, new_file(res->d_name, res->d_type));
    }
    if (t)
        sort_tab_of_file_by_date(files, path, r);
    else
        sort_tab_of_file_by_alpha(files, r);
    
    closedir(dir);

    if (l) {
        struct stat buff;
        int res = stat(path, &buff);
        if(res >= 0) {
            printf("total %lld\n", buff.st_blocks);
        } else {
            printf("%s: Permission denied\n", path);
        }
    }

    // for each file
    for (int i = 0; i < get_tab_of_file_size(files); ++i) {
        switch(files[i]->type) {
            // regular file
            case DT_REG:
                output_file(files[i], path);
                break;
            // directory
            case DT_DIR:
                output_file(files[i], path);
                break;
            // peripheric bloc
            case DT_BLK:
                output_file(files[i], path);
                break;
            // peripheric caract
            case DT_CHR:
                output_file(files[i], path);
                break;
            // FIFO pipe
            case DT_FIFO:
                output_file(files[i], path);
                break;
            // symbolic link
            case DT_LNK:
                output_file(files[i], path);
                break;
            // UNIX socket
            case DT_SOCK:
                output_file(files[i], path);
                break;
            // unknow file
            case DT_UNKNOWN:
                output_file(files[i], path);
                break;
            default:
                fprintf(stderr, "ft_ls: an unexpected error has occured\n");
                exit(EXIT_FAILURE);
                break;
        }
    }
    if (R) {
        for (int i = 0; i < get_tab_of_file_size(files); ++i) {
            if (
                files[i]->type == DT_DIR && 
                strcmp(files[i]->name, ".") != 0 &&
                strcmp(files[i]->name, "..") != 0
            ) {
                char *new_path = strjoin(strjoin(path, "/", FREE_S0), files[i]->name, FREE_S1);
                printf("\n%s:\n", new_path);
                read_dir(new_path);
                free(new_path);
            }
        }
    }
    
    for (int i = 0; i < get_tab_of_file_size(files); ++i) {
        free(files[i]->name);
        free(files[i]);
    }
    free(files);
}

void process(char **argv) {
    int index = optind;

    if (argv[index]) {
        sort(&argv[index]);
        while(argv[index]) {
            if (get_tab_size(&argv[optind]) != 1)
                printf("%s:\n", argv[index]);
            read_dir(argv[index]);
            if (!l || get_tab_size(&argv[optind]) != 1)
                printf("\n");
            ++index;
        }
    } else {
        read_dir(".");
    }
}

int main(int argc, char **argv) {
    fill_opts(argc, argv);
    process(argv);
    exit(EXIT_SUCCESS);
}