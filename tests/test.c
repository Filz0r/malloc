/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:15:06 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/19 16:08:58 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <ft_printf.h>
#include "priv_malloc.h"

int main(void) {
	char *temp = malloc(4);
	char *temp1 = malloc(16000);
	char *temp5 = malloc(450000000);
	char *temp7 = malloc(500000000);
	char *temp6 = malloc(256000000);
	char *temp2 = malloc(10000);
	char *temp34 = malloc(100000);
	char *temp3 = malloc(8);
//	free(temp3);
	char *temp4 = malloc(48);
	print_heap();
	ft_bzero(temp, 4);
	ft_bzero(temp1, 16);
	ft_bzero(temp2, 2);
	ft_bzero(temp3, 8);
	ft_bzero(temp4, 48);
	char *t = NULL;
	for(int k = 0; k < 16; k++) {
		t = malloc(56);
		if (k == 8)
			free(t);
	}
	free(t);
	t = malloc(512);
//	char *temp1 = malloc(2048); // gets a small pool piece
//	print_heap();
//	char *tem2 = malloc(800000); // gets a large pool piece
//	print_heap();
	char ol1[32];
	read(1, ol1, 32);

	temp[0] = 'l';
	temp[1] = 'o';
	temp[2] = 'l';
	ft_memset(temp1, '2', 15);
	ft_memset(temp2, '3', 1);
	ft_memset(temp3, '4', 7);
	ft_memset(temp4, '5', 47);
//	ft_printf("ptr: |%p|\n", temp);
//	ft_printf("ptr1: |%p|\n", temp1);
//	ft_printf("ptr2: |%p|\n", temp2);
//	ft_printf("ptr3: |%p|\n", temp3);
//	ft_printf("ptr4: |%p|\n", temp4);
//	ft_printf("data: |%s|\n", temp);
//	ft_printf("data1: |%s|\n", temp1);
//	ft_printf("data2: |%s|\n", temp2);
//	ft_printf("data3: |%s|\n", temp3);
//	ft_printf("data3: |%s|\n", temp4);

	//	free(temp);
//	print_heap();
	free(temp1);
//	print_heap();
	free(temp2);
	free(temp5);
	print_heap();
//	char olw[32];
//	read(1, olw, 32);

//	free(temp6);
}

