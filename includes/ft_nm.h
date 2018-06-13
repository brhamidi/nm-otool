/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/20 16:31:03 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/13 18:48:21 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <stdio.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>

# include "libft.h"

# define RANLIB_MAGIC	"!<arch>\n"

typedef struct s_sym	t_sym;

struct	s_sym
{
	void			*sym;
	struct s_sym	*next;
};

int 		obj_fat(void *ptr, size_t size, int endian);
int 		obj_fat64(void *ptr, size_t size, int endian);
int 		obj(void *ptr, size_t size, int endian);
int			obj64(void *ptr, size_t size, int endian);
uint64_t	reverse(uint64_t x, uint64_t r, size_t size, int little);
void		free_list(t_sym *head);
int			push_front(t_sym **head, void *e);
void		basic_sort(t_sym *list, const char *strtable, int (*f)(t_sym*, t_sym*, const char *));
int			predicat(t_sym *a, t_sym *b, const char *strtable);

#endif
