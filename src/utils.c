#include "./../ft_ls.h"

char *strjoin(char *s1, char *s2, int fr) {
    char *str = malloc(strlen(s1) + strlen(s2) + 1);
    if(str == NULL)
        return NULL;
    memcpy(str, s1, strlen(s1));
    memcpy(&str[strlen(s1)], s2, strlen(s2));
    str[strlen(s1) + strlen(s2)] = '\0';
    if (fr == FREE_S1)
        free(s1);
    else if (fr == FREE_S2)
        free(s2);
    else if (fr == FREE_S1S2) {
        free(s1);
        free(s2);
    }
    return str;
}

int number_len(int nb) {
    int len = 0;
    if (nb == 0)
        return 1;
    if (nb < 0) {
        nb = -nb;
        ++len;
    }
    while(nb != 0) {
        nb /= 10;
        ++len;
    }
    return len;
}