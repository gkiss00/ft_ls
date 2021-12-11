#include "./../ft_ls.h"

t_file *new_file(char *name, unsigned char type) {
    t_file *file = malloc(sizeof(t_file));
    if (file == NULL)
        return NULL;
    file->name = strdup(name);
    file->type = type;
    return file;
}

int get_tab_of_file_size(t_file **files) {
    int i = 0;
    while(files &&files[i])
        ++i;
    return i;
}

t_file **add_file_to_tab_of_file(t_file **files, t_file *file) {
    int tab_of_file_size = get_tab_of_file_size(files);
    t_file **new_files = malloc((tab_of_file_size + 2) * sizeof(t_file));
    if(new_files == NULL)
        return files;

    int i = 0;
    while (files && files[i]) {
        new_files[i] = files[i];
        ++i;
    }
    new_files[i] = file;
    new_files[i + 1] = NULL;
    free(files);
    return new_files;
}

void sort_tab_of_file_by_alpha(t_file **files, bool reverse) {
    int size = get_tab_of_file_size(files);

    for (int i = 0; i < size - 1; ++i) { 
        bool swapped = false;
        for (int j = 0; j < size - i - 1; ++j) {
            if (!reverse) {
                // reverse
                if (strcmp(files[j]->name, files[j + 1]->name) > 0) {
                    t_file *tmp = files[j];
                    files[j] = files[j + 1];
                    files[j + 1] = tmp;
                    swapped = true;
                }
            } else {
                if (strcmp(files[j]->name, files[j + 1]->name) < 0) {
                    t_file *tmp = files[j];
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

void sort_tab_of_file_by_date(t_file **files, char *path, int date_type, bool reverse) {
    int size = get_tab_of_file_size(files);
    double *datums = malloc(get_tab_of_file_size(files) * sizeof(double));

    for (int i = 0; i < size; ++i) {
        struct stat buff;
        char *new_path = strjoin(strjoin(path, "/", FREE_S0), files[i]->name, FREE_S1);
        int res = stat(new_path, &buff);
        if(res >= 0) {
            if(date_type == MODIFICATION_DATE)
                datums[i] = (double)buff.st_mtimespec.tv_sec + (double)buff.st_mtimespec.tv_nsec / 1000000000;
            else if (date_type == STATUS_DATE)
                datums[i] = (double)buff.st_ctimespec.tv_sec + (double)buff.st_ctimespec.tv_nsec / 1000000000;
            else if (date_type == ACCESS_DATE)
                datums[i] = (double)buff.st_atimespec.tv_sec + (double)buff.st_atimespec.tv_nsec / 1000000000;
        } else {
            printf("%s: Permission denied\n", new_path);
        }
        free(new_path);   
    }

    for (int i = 0; i < size - 1; ++i) { 
        bool swapped = false;
        for (int j = 0; j < size - i - 1; ++j) {
            if (!reverse) {
                // reverse
                if (datums[j] < datums[j + 1]) {
                    t_file *tmp = files[j];
                    files[j] = files[j + 1];
                    files[j + 1] = tmp;
                    double tmp2 = datums[j];
                    datums[j] = datums[j + 1];
                    datums[j + 1] = tmp2;
                    swapped = true;
                }
            } else {
                if (datums[j] > datums[j + 1]) {
                    t_file *tmp = files[j];
                    files[j] = files[j + 1];
                    files[j + 1] = tmp;
                    double tmp2 = datums[j];
                    datums[j] = datums[j + 1];
                    datums[j + 1] = tmp2;
                    swapped = true;
                }
            }
        }
        if(!swapped)
            break;
    }
}