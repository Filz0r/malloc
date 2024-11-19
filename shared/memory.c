/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 23:39:20 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/16 23:39:30 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>


void	ft_bzero(void	*str, size_t length)
{
	unsigned char	*string;
	size_t 			i;

	i = 0;
	string = (unsigned char *)str;
	while (string && i++ < length)
		*string++ = '\0';
}

void	*ft_memset(void *str, int c, size_t length)
{
	unsigned char	*string;

	string = (unsigned char *)str;
	while (length--)
		*string++ = (unsigned char)c;
	return (str);
}