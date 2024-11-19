/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:39:53 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/19 13:30:31 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>
#include <print_utils.h>

int	ft_printf(const char *buffer, ...)
{
	va_list	args;
	int		k;
	int		flag;

	k = 0, flag = 0;
	va_start(args, buffer);
	while (*buffer != 0)
	{
		if (*buffer == '%')
		{
			k += ft_type_parser(++buffer, &flag, args);
			buffer++;
			if (flag == 1) {
				buffer++;
				flag = 0;
			}
		}
		else
			k += ft_putchar(*buffer++);
	}
	va_end(args);
	return (k);
}