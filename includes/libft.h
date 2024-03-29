/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sotherys <sotherys@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 14:06:54 by sotherys          #+#    #+#             */
/*   Updated: 2022/07/29 09:42:24 by sotherys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

# include "bool.h"

# define PI 3.14159265359
# define EPS 0.00001

t_bool	ft_malloc(void **ptr, size_t n);
int		ft_abs(int x);
double	ft_fabs(double x);
int		ft_min(int a, int b);
int		ft_max(int a, int b);
double	ft_fmin(double a, double b);
double	ft_fmax(double a, double b);
double	ft_fpow(double x, int pow);
void	ft_swap(int *x, int *y);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);

char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);

char	**ft_split(const char *str, char c);
size_t	ft_split_word_count(const char *str, char c);
void	ft_split_free(char **out);

int		ft_isdigit(int c);
int		ft_atoi(const char *str);

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}				t_list;

t_list	*lst_new(void *data);
void	lst_add_front(t_list **lst, t_list *new);
t_list	*lst_find(t_list *lst, void *data, int (*cmp)(void *, void *));

#endif
