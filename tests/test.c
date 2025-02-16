/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:15:06 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/22 12:28:01 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <ft_printf.h>
#include "priv_malloc.h"

int main(void) {
	char *temp = malloc(4);
	ft_printf("%p\n", temp);
	char *temp3 = malloc(25);
	ft_printf("%p\n", temp3);
	void *temp2 = 0;
	for (int i = 0; i < 16389; i++) {
		temp2 = malloc(10);
//		ft_printf("t2: %p\n", temp2);
	}
	print_heap();
	char ol1[32];
	read(1, ol1, 32);
}

