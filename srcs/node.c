#include "./../ft_ls.h"

void add_root(t_data *data, t_file *new) {
    t_file  **tmp = data->files;
    int     actual_size = file_nb_child2(data->files);
    int     i = 0;

    data->files = malloc((sizeof(t_file *) * actual_size) + 2);
    while(i < actual_size) {
        data->files[i] = tmp[i];
        ++i;
    }
    data->files[i] = new;
    data->files[i + 1] = NULL;
    free(tmp);
}

t_node *new_node(char *target) {
    t_node *node;

    node = malloc(sizeof(t_node));
    node->target = strdup(target);
    node->next = NULL;
    return (node);
}

t_node *node_last(t_node *node) {
    while (node && node->next) {
        node = node->next;
    }
    return (node);
}

void node_add_back(t_node **head, t_node *new) {
    if (head != 0)
	{
		if (*head && new != 0)
		{
			node_last(*head)->next = new;
		}
		else
		{
			*head = new;
		}
	}
}

void free_node(t_node *node) {
    t_node *tmp = node;

    while(node) {
        tmp = node->next;
        free(node->target);
        free(node);
        node = tmp;
    }
}

t_file *new_file(char *name, int type, int p, char *owner, char *team, int size, int c_time, int u_time, int r_time, bool a) {
    t_file *file;

    file = malloc(sizeof(t_file));
    file->name = strdup(name);
    file->type = type;
    file->permissions = p;
    file->nb_link = 0;
    file->owner = strdup(owner);
    file->team = strdup(team);
    file->size = size;
    file->c_time = c_time;
    file->u_time = u_time;
    file->r_time = r_time;
    file->accessibility = a;
    file->childs = NULL;
    return (file);
}

t_file *new_empty_file() {
    t_file *file;

    file = malloc(sizeof(t_file));
    file->name = NULL;
    file->type = 0;
    file->permissions = 0;
    file->nb_link = 0;
    file->owner = NULL;
    file->team = NULL;
    file->size = 0;
    file->c_time = 0;
    file->u_time = 0;
    file->r_time = 0;
    file->accessibility = false;
    file->childs = NULL;
    return (file);
}

int file_nb_child2(t_file **file) {
    t_file **tmp = file;
    int     i = 0;
    while(tmp && tmp[i]) {
        ++i;
    }
    return (i);
}

int file_nb_child(t_file *file) {
    int     i = 0;
    while(file->childs && file->childs[i]) {
        ++i;
    }
    return (i);
}

void file_add_child(t_file *file, t_file *child) {
    t_file  **tmp = file->childs;
    int     actual_size = file_nb_child(file);
    int     i = 0;

    file->childs = malloc((sizeof(t_file *) * actual_size) + 2);
    while(i < actual_size) {
        file->childs[i] = tmp[i];
        ++i;
    }
    file->childs[i] = child;
    file->childs[i + 1] = NULL;
    free(tmp);
}

void free_file(t_file *file) {
    // free(file->name);
    // free(file->owner);
    // free(file->team);
    int i = 0;
    while(file->childs && file->childs[i]) {
        free_file(file->childs[i]);
        ++i;
    }
    // free(file->childs);
    // free(file);
}

void free_data(t_data *data) {
    free_node(data->target);
    int i = 0;
    while(data->files && data->files[i]) {
        free_file(data->files[i]);
        ++i;
    }
    //free(data->files);
}