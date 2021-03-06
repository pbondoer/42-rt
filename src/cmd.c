/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbondoer <pbondoer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 12:14:54 by pbondoer          #+#    #+#             */
/*   Updated: 2017/01/30 14:15:56 by pbondoer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_cmd		*cmd(void)
{
	static t_cmd	cmd;
	
	return (&cmd);
}

int			command_line(int argc, char **argv)
{
	int i;

	i = 1;
	cmd()->output = NULL;
	cmd()->scene = NULL;
	while (i < argc)
	{
		if (argv[i][0] == '-')
		{
			if (i + 1 >= argc)
				return (1);
			if (argv[i][1] == 'o')
				cmd()->output = argv[i + 1];
			i++;
		}
		else
			cmd()->scene = argv[i];
		i++;
	}
	if (cmd()->scene == NULL)
		return (1);
	return (argc == 1);
}
