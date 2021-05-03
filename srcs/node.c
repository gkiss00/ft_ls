#include "./../ft_ls.h"

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
    file->next = NULL;
    return (file);
}

t_file *file_last(t_file *file) {
    while (file && file->next) {
        file = file->next;
    }
    return (file);
}

void file_add_back(t_file **head, t_file *new) {
    if (head != 0)
	{
		if (*head && new != 0)
		{
			file_last(*head)->next = new;
		}
		else
		{
			*head = new;
		}
	}
}

void free_file(t_file *file) {
    t_file *tmp = file;

    while(file) {
        tmp = file->next;
        free(file->name);
        free(file->owner);
        free(file->team);
        free(file);
        file = tmp;
    }
}