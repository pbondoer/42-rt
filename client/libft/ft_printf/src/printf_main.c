/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmarot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/14 12:04:11 by hmarot            #+#    #+#             */
/*   Updated: 2016/06/19 11:42:09 by hmarot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int		print(const char *format, ...)
{
	char		*form;
	t_penv		env;
	int			i;
	va_list		ap;

	va_start(ap, format);
	form = (char *)format;
	prf_init(&env);
	i = 0;
	while (form[i])
	{
		prf_loop(&env);
		i = prf_main_b(form, &env, i, ap);
		if (i == -1)
			return (-1);
	}
	write(1, env.buffer, env.pos);
	va_end(ap);
	return (env.ret + env.pos);
}

int		prf_main_b(char *form, t_penv *env, int i, va_list ap)
{
	if (form[i] != '%')
		push_buff(form[i], env);
	else
	{
		i = prf_flag(form, i + 1, env);
		if (i == -1)
			return (-1);
		i = prf_field(form, i, env);
		if (i == -1)
			return (-1);
		i = prf_pressi(form, i, env);
		if (i == -1)
			return (-1);
		i = prf_modif(form, i, env);
		if (i == -1)
			return (-1);
		i = prf_conv(form, i, env, ap);
		if (i == -1)
			return (-1);
	}
	++i;
	if (env->err)
		return (-1);
	return (i);
}
