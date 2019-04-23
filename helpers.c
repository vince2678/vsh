#include <string.h> /* strtok */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* perror */

int count_char(char *str, char c)
{
    int count = 0;

    char *next = str;

    while(*next)
    {
        if (*next == c)
            count += 1;
        next++;
    }

    return count;
}

char **strtostrv(char *str, char sep)
{
    char delim[2];
    char *token;
    char **strv, **strv_i;
    int count;

    if (str == NULL)
        return NULL;

    count = count_char(str, sep) + 2;

    strv = malloc(sizeof(char*) * count);

    if (strv == NULL)
    {
        perror("malloc");
        return NULL;
    }

    strv_i = strv;

    sprintf(delim, "%c", sep);

    token = strtok(str, delim);

    while(token)
    {
        *strv_i = token;
        token = strtok(NULL, delim);
        strv_i = strv_i + 1;
    }

    *strv_i = (char *) NULL;

    return strv;
}

int strvlen(char **strv)
{
    char **strv_i;
    int len;

    strv_i = strv;
    len = 0;

    while (*strv_i)
    {
        len = len + 1;
        strv_i = strv_i + 1;
    }
    return len;
}