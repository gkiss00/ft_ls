#include "ft_ls.h"

//DEV
int fd = -1;

// FLAGS
bool l = false;
bool R = false;
bool a = false;
bool r = false;
bool t = false;
bool o = true;
bool f = true;
bool n = false;
bool i = false;
int date_type = MODIFICATION_DATE;

// COLUMN SIZE
int inode_width = 0;
int symbolic_link_width = 0;
int user_width = 0;
int group_width = 0;
int file_size_width = 0;


/* * * * * * * * * * * * * * * * * * * * *

*               PARSING                  *

* * * * * * * * * * * * * * * * * * * * */

void fill_opts(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "lRartcuofni")) != -1) {
        switch (opt) {
        case 'l':
            l = true;
            break;
        case 'o':
            l = true;
            o = false;
            break;
        case 'n':
            l = true;
            n = true;
            break;
        case 'R':
            R = true;
            break;
        case 'a':
            a = true;
            break;
        case 'f':
            a = true;
            f = false;
            break;
        case 'r':
            r = true;
            break;
        case 't':
            t = true;
            break;
        case 'i':
            i = true;
            break;
        case 'c':
            t = true;
            date_type = STATUS_DATE;
            break;
        case 'u':
            t = true;
            date_type = ACCESS_DATE;
            break;
        default: /* aq?aq */
            fprintf(stderr, "Usage: %s [-lRartcuofni] <path>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * *

*                UTILS                   *

* * * * * * * * * * * * * * * * * * * * */

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

/* * * * * * * * * * * * * * * * * * * * *

*               GETTER                   *

* * * * * * * * * * * * * * * * * * * * */

int get_user_len(uid_t id) {
    if(!n) {
        struct passwd *pwd;
        pwd = getpwuid(id);
        return strlen(pwd->pw_name);
    } else {
        return number_len(id);
    }
}

int get_group_len(gid_t id) {
    if(!n) {
        struct group *group;
        group = getgrgid(id);
        return strlen(group->gr_name);
    } else {
        return number_len(id);
    }
}

/* * * * * * * * * * * * * * * * * * * * *

*               OUTPUT                   *

* * * * * * * * * * * * * * * * * * * * */

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
    if ((mode & S_ISUID) == S_ISUID)
        printf("s");
    else if ((mode & S_IXUSR) == S_IXUSR)
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

void output_user(uid_t id) {
    if(!n) {
        struct passwd *pwd;
        pwd = getpwuid(id);
        if(pwd)
            printf(" %-*s", user_width + 1, pwd->pw_name);
    } else {
        printf(" %-*d", user_width + 1, id);
    }
}

void output_group(gid_t id) {
    if (o) {
        if(!n) {
            struct group *group;
            group = getgrgid(id);
            if(group)
                printf(" %-*s", group_width + 1, group->gr_name);
        } else {
            printf(" %-*d", group_width + 1, id);
        }
    }
}

void ouput_timestamp(struct stat time) {
    char *timestamp = NULL;
    if (date_type == MODIFICATION_DATE)
        timestamp = ctime(&time.st_mtimespec.tv_sec);
    else if (date_type == STATUS_DATE)
        timestamp = ctime(&time.st_ctimespec.tv_sec);
    else if (date_type == ACCESS_DATE)
        timestamp = ctime(&time.st_atimespec.tv_sec);
    char tmp[strlen(timestamp)];
    memset(tmp, 0, strlen(timestamp));
    memcpy(tmp, timestamp, strlen(timestamp) - 1);
    printf(" %s", tmp);
}

void output_name(t_file *file, char *symlink, mode_t mode) {
    printf(" ");
    if (file->type == DT_DIR) { // DIRECTORY
        printf(BOLD_CYAN);
    } else if (file->type == DT_CHR) { // CHARACTER DEVICE
        printf(BLUE_ON_YELLOW);
    } else if (file->type == DT_BLK) { // BLOCK DEVICE
        printf(BLUE_ON_CYAN);
    } else if (file->type == DT_FIFO) { // NAMED PIPE
        printf(YELLOW);
    } else if (file->type == DT_LNK) { // SYMBOLIC LINK
        printf(PURPLE);
    } else if (file->type == DT_REG && (mode & S_ISUID) == S_ISUID) { // FILE
        printf(BLACK_ON_RED);
    } else if (file->type == DT_REG && (mode & S_IXUSR) == S_IXUSR) { // FILE
        printf(RED);
    } else if (file->type == DT_SOCK) { // UNIX DOMAIN SOCKET
        printf(GREEN);
    } else if (file->type == DT_UNKNOWN) { // UNKNOWN
        printf(BOLD_CYAN);
    }
    printf("%s", file->name);
    printf(WHITE);
    if(file->type == DT_LNK && symlink != NULL)
        printf(" -> %s", symlink);
}

void output_file(t_file *file, char *path) {
    if (a || (!a && file->name[0] != '.')) {
        struct stat buff;
        char *new_path = strjoin(strjoin(path, "/", FREE_S0), file->name, FREE_S1);
        int stat_res = stat(new_path, &buff);
        if (l) {
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
                if(i)
                    printf("%*lld ", inode_width, buff.st_ino);
                output_type(file->type);
                output_permissions(buff.st_mode);
                if(strcmp(new_path, "..") == 0 || strcmp(new_path, "./..") == 0)
                    printf("@");
                else
                    printf(" ");
                printf("%*d", symbolic_link_width + 1, (int)buff.st_nlink);
                output_user(buff.st_uid);
                output_group(buff.st_gid);
                printf("%*ld", file_size_width + 1, (long)buff.st_size);
                ouput_timestamp(buff);
                output_name(file, symlink, buff.st_mode);
                printf("\n");
            } else {
                printf("%s: Permission denied\n", new_path);
            }
            free(new_path);
            free(symlink);
        } else {
            if(i)
                printf(" %lld ", buff.st_ino);
            output_name(file, NULL, buff.st_mode);
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * *

*               PROCESS                  *

* * * * * * * * * * * * * * * * * * * * */

void read_dir(char *path) {
    DIR *dir = opendir(path);
    if(dir == NULL) {
        fprintf(stderr, "ft_ls: %s: No such file or directory", path);
        return ;
    }

    int total = 0;
    t_file **files = NULL;
    struct dirent *res = NULL;
    inode_width = 0;
    symbolic_link_width = 0;
    user_width = 0;
    group_width = 0;
    file_size_width = 0;
    while((res = readdir(dir)) != NULL) {
        files = add_file_to_tab_of_file(files, new_file(res->d_name, res->d_type));
        struct stat buff;
        if(l){
            char *new_path = strjoin(strjoin(path, "/", FREE_S0), res->d_name, FREE_S1);
            int stat_res = stat(new_path, &buff);
            free(new_path);
            if (a || (!a && res->d_name[0] != '.'))
                if (stat_res >= 0)
                    total += (int)buff.st_blocks;
            if (number_len(buff.st_ino) > inode_width)
                inode_width = number_len(buff.st_ino);
            if (number_len(buff.st_nlink) > symbolic_link_width)
                symbolic_link_width = number_len(buff.st_nlink);
            if (number_len(buff.st_size) > file_size_width)
                file_size_width = number_len(buff.st_size);
            if (get_user_len(buff.st_uid) > user_width)
                user_width = get_user_len(buff.st_uid);
            if (get_group_len(buff.st_gid) > group_width)
                group_width = get_group_len(buff.st_gid);
        }
    }
    if (f) {
        if (t)
            sort_tab_of_file_by_date(files, path, date_type, r);
        else
            sort_tab_of_file_by_alpha(files, r);
    }

    closedir(dir);

    if (l)
        printf("total %d\n", total);

    for (int i = 0; i < get_tab_of_file_size(files); ++i)
        output_file(files[i], path);

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