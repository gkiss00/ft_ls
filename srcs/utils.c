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

void to_lower(char *str) {
    int i = 0;
    while(str && str[i]){
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + ('a' - 'A');
        ++i;
    }
}

char		*ft_substr(char *s, int start, size_t len)
{
	int		i;
	char				*str;

	i = 0;
	if (!s || !(str = malloc((len + 1))))
		return (0);
	if (start < (int)ft_strlen(s))
	{
		while (s[start] && len > 0)
		{
			str[i] = s[start];
			++i;
			++start;
			--len;
		}
	}
	str[i] = '\0';
	return (str);
}

static char			**liberer(char **tab, int j)
{
	--j;
	while (j >= 0)
	{
		free(tab[j]);
		--j;
	}
	return (0);
}

static int			separation(char c, char charset)
{
	if (c == charset)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

static int			count_word(char *str, char charset)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && separation(str[i], charset) == 1)
		{
			++i;
		}
		if (str[i] != '\0' && separation(str[i], charset) == 0)
		{
			++k;
			while (str[i] != '\0' && separation(str[i], charset) == 0)
				++i;
		}
	}
	return (k);
}

static char			*mot(char *str, char charset)
{
	char	*mot;
	int		i;

	i = 0;
	while (str[i] != '\0' && separation(str[i], charset) == 0)
	{
		++i;
	}
	if (!(mot = malloc(sizeof(char*) + (i + 1))))
	{
		return (0);
	}
	i = 0;
	while (str[i] != '\0' && separation(str[i], charset) == 0)
	{
		mot[i] = str[i];
		++i;
	}
	mot[i] = '\0';
	return (mot);
}

char				**ft_split(const char *str, char charset)
{
	char	**tab;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (str == 0)
		return (0);
	if (!(tab = malloc(sizeof(char*) * (count_word((char*)str, charset) + 1))))
		return (0);
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && separation(str[i], charset) == 1)
			++i;
		if (str[i] != '\0' && separation(str[i], charset) == 0)
		{
			if ((tab[j] = mot((char*)&str[i], charset)) == 0)
				return (liberer(tab, j));
			++j;
			while (str[i] != '\0' && separation(str[i], charset) == 0)
				++i;
		}
	}
	tab[count_word((char*)str, charset)] = NULL;
	return (tab);
}