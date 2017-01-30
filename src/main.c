/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/09 09:15:54 by hmartzol          #+#    #+#             */
/*   Updated: 2017/01/30 14:18:33 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <rt.h>

int			main(const int argc, char **argv, char **env)
{
	int		fd;
	char	*src;

	ft_init(env);
	if (command_line(argc, argv))
		ft_end(0 * ft_printf("\nUsage: \t%s/%s <scene.json>\n\n", ft_pwd(),
					ft_path_name(argv[0])));
	if ((fd = open(cmd()->scene, O_RDONLY)) == -1)
		ft_end(-1);
	parser(src = ft_readfile(fd));
	close(fd);
	ft_free(src);
	if ((fd = open(OCL_SOURCE_PATH, O_RDONLY)) == -1)
		ft_end(-1);
	ftocl_make_program(ft_str_to_id64("rt"),
			src = ft_str_clear_commentaries(ft_readfile(fd)), NULL);
	close(fd);
	if (!(fd = ftocl_set_current_kernel(ft_str_to_id64("rt_kernel"))))
		rt();
	ft_free(src);
	if (fd == 1)
		ft_end(0 * ft_printf("kernel was not found\n"));
	ft_end(0);
}
