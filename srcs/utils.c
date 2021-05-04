#include "./../ft_ls.h"

int ft_strlen(char *str) {
    int i = 0;
    while(str && str[i])
        ++i;
    return i;
}

char *ft_strjoin(char *s1, char *s2) {
    char *res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
    int i = 0;
    while(s1 && s1[i]){
        res[i] = s1[i];
        ++i;
    }
    int j = 0;
    while(s2 && s2[j]) {
        res[i + j] = s2[j];
        ++j;
    }
    res[i + j] = '\0';
    return res;
}