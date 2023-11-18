/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stddef.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <stdarg.h>
# include <limits.h>
/*#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>*/

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}		t_list;

typedef struct s_flags
{
	va_list	args;
	char	specifier_type;
	char	*string;
	int		len;
	long	n;
}				t_flags;

int		ft_printf(const char *format, ...);
void		ft_pf_f_init(t_flags *f);
const char	*ft_pf_type(t_flags *f, char *s);

/*##################~~~~~AUX~~~~~~###################*/
int			ft_putchar(char c);
int			ft_putstr(char *str);
int			ft_strcmp(char *s1, char *s2);

/*##################~~~~~NUMBERS~~~~~~###################*/
void		type_i(t_flags	*f);
void		ft_diux(t_flags *f);

/*##################~~~~~HEXA~~~~~~###################*/
void		hexa(unsigned long nbr, char *base, unsigned int size, t_flags *f);

/*##################~~~~~CSP~~~~~~###################*/
void		type_s(t_flags *f, char *str);

char		*ft_itoa_base(unsigned long int value, int base);

char	*get_next_line_bonus(int fd);
char	*get_next_line(int fd);
size_t	ft_strlen_get(const char *str);
char	*ft_strjoin_get(char *s1, char *s2);
int		ft_isolate_line(char *str);

int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
int				ft_tolower(int c);
int				ft_toupper(int c);
int				ft_atoi(const char	*str);
int				ft_strncmp(const char *str1, const char *str2, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);

void			*ft_memset(void *ptr, int value, size_t num);
void			*ft_memcpy(void *dest, const void *src, size_t num);
void			*ft_memmove(void *dest, const void *src, size_t num);
void			*ft_memchr(const void *str, int d, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
void			ft_bzero(void *ptr, size_t num);
void			ft_striteri(char *s, void (*f)(unsigned int, char*));
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);

size_t			ft_strlen(const char *str);
size_t			ft_strlcat(char *dest, const char *src, size_t d_size);
size_t			ft_strlcpy(char *dest, const char *src, size_t d_size);

char			*ft_strchr(const char *str, int d);
char			*ft_strrchr(const char *str, int d);
char			*ft_strnstr(const char *dest, const char *src, size_t len);
char			*ft_strdup(const char	*s);
char			*ft_substr(char const	*s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			**ft_split(char const *s, char c);
char			*ft_itoa(int n);

/*________________BONUS________________*/

t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstmap(t_list *ls, void *(*f)(void *), void (*del)(void *));
t_list			*ft_lstnew(void *content);

int				ft_lstsize(t_list *lst);

void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstadd_front(t_list **lst, t_list *new);
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void *));

#endif
