#ifndef FT_LS_H
#define FT_LS_H

# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <assert.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <errno.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <math.h>
# include <stdbool.h>
# include <dirent.h>
# include <time.h>
# include <sys/xattr.h>
# include <grp.h>

typedef struct          s_node
{
    char                *target;
    struct s_node       *next;
}                       t_node;

typedef struct          s_file
{
    char                *name; // ok
    int                 type; // ok
    int                 permissions; // ok
    int                 nb_link;
    char                *owner; // ok
    char                *team; // ok
    int                 size; // ok
    ino_t               node_id; // ok
    time_t              c_time; // ok
    time_t              u_time; // ok
    time_t              r_time; // ok
    bool                accessibility;
    struct s_file       **childs; // ok
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
    bool                i; // display node id ||OUTPUT
    bool                u; // use date of last access instead of modification date for -l -t || SORTING && OUPUT
    bool                q; // print question mark instead of non printable caract in file name || OUTPUT
    bool                one; // 1 file by line || OUTPUT
}                       t_option;

typedef struct		    s_data
{
    struct s_option     opts;
    struct s_node       *target;
    struct s_file       **files;
}                       t_data;

//NODE
t_node *new_node(char *target);
t_node *node_last(t_node *node);
void node_add_back(t_node **head, t_node *new);
void free_node(t_node *port);

t_file *new_file(char *name, int type, int p, char *owner, char *team, int size, int c_time, int u_time, int r_time, bool a);
t_file *new_empty_file();
void file_add_child(t_file *file, t_file *child);
int file_nb_child(t_file *file);
int file_nb_child2(t_file **file);
void free_file(t_file *file);

void add_root(t_data *data, t_file *new);
void free_data(t_data *data);

//UTILS
char *ft_strjoin(char *s1, char *s2);
char **ft_split(const char *str, char charset);
void to_lower(char *str);
char *ft_substr(char *s, int start, size_t len);

//ERROR

//INIT
void    init(t_data *data);

//PARSING
void parsing(t_data *data, char **argv);

//SORTING
void sort(t_data *data);

//OUTPUT
void output(t_data *data);

#endif