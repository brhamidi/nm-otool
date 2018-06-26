/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhamidi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:56 by bhamidi           #+#    #+#             */
/*   Updated: 2018/06/26 15:25:16 by bhamidi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

const char            *get_cputype(cpu_type_t cputype)
{
	const cpu_type_t	cpu_tab[] = {
		CPU_TYPE_VAX,
		CPU_TYPE_MC680x0,
		CPU_TYPE_X86,
		CPU_TYPE_I386,
		CPU_TYPE_X86_64,
		CPU_TYPE_MC98000,
		CPU_TYPE_HPPA,
		CPU_TYPE_ARM,
		CPU_TYPE_SPARC,
		CPU_TYPE_POWERPC,
		CPU_TYPE_I860,
		CPU_TYPE_POWERPC64
	};
	const char	*cpu_name_tab[] = {
		"vax",
		"mc680",
		"i386",
		"i386",
		"x86_64",
		"mc98000",
		"hppa",
		"arm",
		"sparc",
		"ppc",
		"i860",
		"ppc64"
	};
	unsigned long i = -1;

	while (++i < sizeof(cpu_name_tab) / sizeof(*cpu_name_tab))
		if (cpu_tab[i] == cputype)
			return (cpu_name_tab[i]);
	return "?";
}

int obj_fat(t_info *inf)
{
	struct fat_header	*fheader;
	struct fat_arch		*farch;
	unsigned int		i;
	t_info				new;

	if (check(inf, inf->ptr, sizeof(*fheader) + sizeof(*farch)))
		return (-1);
	fheader = (struct fat_header *)inf->ptr;
	farch = (struct fat_arch *) (fheader + 1);
	i = 0;
	while (i < rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian))
	{
		if (inf->mode == FULL && i != rev(fheader->nfat_arch, 0, sizeof(uint32_t), inf->endian))
			ft_putchar('\n');
		if (check(inf, farch + i, sizeof(*farch)))
			return (-1);
		if (rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			inf->mode = SINGLE;
		new.ptr = inf->ptr + rev(farch[i].offset, 0, sizeof(uint32_t), inf->endian);
		new.end = new.ptr + rev(farch[i].size, 0, sizeof(uint32_t), inf->endian);
		new.file_name = inf->file_name;
		if (check(inf, new.ptr, 0) || check(inf, new.end, 0))
			return (-1);
		if (inf->mode == SINGLE && rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian) == CPU_TYPE_X86_64)
			return analyse_object(& new);
		if (inf->mode == FULL)
		{
			ft_putstr(inf->file_name);
			ft_putstr(" (for architecture ");
			ft_putstr(get_cputype(rev(farch[i].cputype, 0, sizeof(cpu_type_t), inf->endian)));
			ft_putendl("):");
			analyse_object(& new);
		}
		i++;
	}
	if (inf->mode == FULL)
		return (0);
	inf->mode = FULL;
	return obj_fat(inf);
}

int obj_fat64(t_info *inf)
{
	(void)inf;
	return 43;
}
