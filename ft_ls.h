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
    char                *name;
    int                 type;
    int                 permissions;
    int                 nb_link;
    char                *owner;
    char                *team;
    int                 size;
    int                 c_time;
    int                 u_time;
    int                 r_time;
    bool                accessibility;
    struct s_file       *next;
}                       t_file;

typedef struct          s_option
{
    bool                R; // recursive in dir
    bool                r; // reverse the sorting
    bool                a; // show hidden files
    bool                l; // show details
    bool                t; // sort by date
    bool                C; // sort vertical
    bool                F; // add a charact after file *for exe /for dir @link |fifos
    bool                c; // use date of status change instead of modification date for -l -t
    bool                d; // display repertory as file (without showing what's inside)
    bool                i; // display node id
    bool                u; // use date of last access instead of modification date for -l -t
    bool                q; // print question mark instead of non printable caract in file name
    bool                one; // 1 file by line
}                       t_option;

typedef struct		    s_data
{
    struct s_option     opts;
    struct s_node       *target;
}                       t_data;

//NODE
t_node *new_node(char *target);
t_node *node_last(t_node *node);
void node_add_back(t_node **head, t_node *new);
void free_node(t_node *port);

t_file *new_file(char *name, int type, int p, char *owner, char *team, int size, int c_time, int u_time, int r_time, bool a);
t_file *file_last(t_file *file);
void file_add_back(t_file **head, t_file *new);
void free_file(t_file *file);

//ERROR

//INIT
void    init(t_data *data);

//PARSING
void parsing(t_data *data, char **argv);

#endif