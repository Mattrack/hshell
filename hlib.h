#ifndef __HLIB_H__
#define __HLIB_H__

int		hstrcmp(const char *a, const char *b);
int		hstrncmp(const char *a, const char *b, int n);
int		hstrlen(const char* str);
char	*hstrdup(const char *str);
int		hprintf(const char* str, ...);
char	*hstrtok_r(char *str, const char *delim, char **saveptr);
const char *hstrchr(const char *str, int c);
char	*hstrcat(char* dest, const char* src);
char	*hstrcpy(char* dest, const char* src);
int		hatoi(const char* str);

#endif