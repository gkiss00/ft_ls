#ifndef FT_LS_H
#define FT_LS_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <assert.h>
# include <netdb.h>
# include <errno.h>
# include <math.h>
# include <stdbool.h>
# include <dirent.h>
# include <time.h>
# include <grp.h>
# include <pwd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <sys/xattr.h>

# include "include/color.h"

# define FREE_S0 0
# define FREE_S1 1
# define FREE_S2 2
# define FREE_S1S2 3

# define SORT_BY_ALPHA 1
# define SORT_BY_DATE 2

typedef struct          s_file
{
    char *name;
    unsigned char type;
}                       t_file;

typedef struct          s_option
{
    bool                R; // recursive in dir || OUTPUT
    bool                r; // reverse the sorting  || SORTING
    bool                a; // show hidden files || OUTPUT
    bool                l; // show details || OUTPUT
    bool                t; // sort by date || SORTING
    bool                C; // sort vertical || SORTING ???
    bool                F; // add a charact after file *for exe /for dir @link |fifos || OUTPUT
    bool                c; // use date of status change instead of modification date for -l -t || SORTING && OUPUT
    bool                d; // display repertory as file (without showing what's inside) || OUTPUT
    bool                i; // display node id || OUTPUT
    bool                u; // use date of last access instead of modification date for -l -t || SORTING && OUPUT
    bool                q; // print question mark instead of non printable caract in file name || OUTPUT
    bool                one; // 1 file by line || OUTPUT
}                       t_option;

t_file *new_file(char *name, unsigned char type);
int get_tab_of_file_size(t_file **files);
t_file **add_file_to_tab_of_file(t_file **files, t_file *file);

// bubble sort
void sort_tab_of_file_by_alpha(t_file **files, bool reverse);
void sort_tab_of_file_by_date(t_file **files, char *path, bool reverse);

char *strjoin(char *s1, char *s2, int fr);

#endif