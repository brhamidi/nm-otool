/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:49:21 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/17 15:43:49 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char	*arch_name[4] = {
	"I386",
	"X86_64",
	"PPC",
	"PPC64"
};

void	display_info64(const t_sym *node, t_info *inf, const char *strtable)
{
	const struct nlist_64	*list;

	if (!node)
		return;
	if (check(inf, node->sym, sizeof(*list)))
		return;
	list = (struct nlist_64 *)node->sym;
	if ((rev(list->n_type, 0, 1, inf->endian) & N_TYPE) == N_UNDF)
		ft_putnchar(' ', 16);
	else
		put_value(rev(list->n_value, 0, sizeof(uint64_t), inf->endian), 16);
	ft_putchar(' ');
	ft_putchar('X');
	ft_putchar(' ');
	ft_putendl(strtable + rev(list->n_un.n_strx, 0, sizeof(uint32_t), inf->endian));
	return display_info64(node->next, inf, strtable);
}

void	display_info(const t_sym *node, t_info *inf, const char *strtable)
{
	const struct nlist	*list;

	if (!node)
		return;
	if (check(inf, node->sym, sizeof(*list)))
		return;
	list = (struct nlist *)node->sym;
	if ((rev(list->n_type, 0, 1, inf->endian) & N_TYPE) == N_UNDF)
		ft_putnchar(' ', 8);
	else
		put_value(rev(list->n_value, 0, sizeof(uint32_t), inf->endian), 8);
	ft_putchar(' ');
	ft_putchar('X');
	ft_putchar(' ');
	ft_putendl(strtable + rev(list->n_un.n_strx, 0, sizeof(uint32_t), inf->endian));
	return display_info(node->next, inf, strtable);
}

int		ft_nlist(t_info *inf, struct nlist *nlist, uint32_t nsyms, uint32_t stroff)
{
	if (!nsyms)
	{
		basic_sort(inf->list, inf->ptr + stroff, predicat64);
		display_info(inf->list, inf, inf->ptr + stroff);
		free_list(inf->list);
		return (0);
	}
	if (check(inf, nlist, sizeof(*nlist)))
		return (-1);
	push_front(& inf->list, nlist);
	return (ft_nlist(inf, nlist + 1, nsyms - 1, stroff));
}

int		ft_nlist64(t_info *inf, struct nlist_64 *nlist, uint32_t nsyms, uint32_t stroff)
{
	if (!nsyms)
	{
		basic_sort(inf->list, inf->ptr+ stroff, predicat64);
		display_info64(inf->list, inf, inf->ptr + stroff);
		free_list(inf->list);
		return (0);
	}
	if (check(inf, nlist, sizeof(*nlist)))
		return (-1);
	push_front(& inf->list, nlist);
	return (ft_nlist64(inf, nlist + 1, nsyms - 1, stroff));
}

int		symtab(t_info *inf, void *curr)
{
	struct symtab_command	*sym;
	uint32_t				symoff;
	uint32_t				stroff;
	uint32_t				nsyms;
	const size_t			size_nlist = sizeof(struct nlist);
	const size_t			size_nlist64 = sizeof(struct nlist_64);

	if (check(inf, curr, sizeof(*sym)))
		return (-1);
	sym = (struct symtab_command*)curr;
	symoff = rev(sym->symoff, 0, sizeof(uint32_t), inf->endian);
	stroff = rev(sym->stroff, 0, sizeof(uint32_t), inf->endian);
	nsyms = rev(sym->nsyms, 0, sizeof(uint32_t), inf->endian);
	if (check(inf, inf->ptr + symoff, inf->arch == I386 ? size_nlist : size_nlist64))
		return (-1);
	if (inf->arch == I386)
		return (ft_nlist(inf, inf->ptr + symoff, nsyms, stroff));
	else
		return (ft_nlist64(inf, inf->ptr + symoff, nsyms, stroff));
}

int 	load(t_info *inf, void *curr, size_t ncmds, size_t acc)
{
	struct load_command	*lc;

	if (acc == rev(ncmds, 0, sizeof(size_t), inf->endian))
		return (-1);
	if (check(inf, curr, sizeof(*lc)))
		return (-1);
	lc = (struct load_command *)curr;
	if (rev(lc->cmd, 0, sizeof(uint32_t), inf->endian) == LC_SYMTAB)
		return (symtab(inf, curr));
	return (load(inf, curr + lc->cmdsize, ncmds, acc + 1));
}

int		obj(t_info *inf)
{
	struct mach_header	*header;

	if (check(inf, inf->ptr, sizeof(*header)))
		return (-1);
	inf->arch = I386;
	header = (struct mach_header*)inf->ptr;
	return (load(inf, inf->ptr + sizeof(*header), header->ncmds, 0));
}

int		obj64(t_info *inf)
{
	struct mach_header_64	*header;

	if (check(inf, inf->ptr, sizeof(*header)))
		return (-1);
	inf->arch = X86_64;
	header = (struct mach_header_64*)inf->ptr;
	return load(inf, inf->ptr + sizeof(*header), header->ncmds, 0);
}
