/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 23:44:38 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/22 10:25:37 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>
#include <ft_printf.h>

void print_heap(void)
{
	if (!heap.page_size) {
		ft_printf("bye\n");
		return ;
	}
	ft_printf("========%sStart Heap Print%s========\n", BLUE, WHITE);
	ft_printf("Page Size: %zu\n", heap.page_size);
	ft_printf("Total Memory: %zu in bytes\n", heap.total_memory);
//	print_tiny_pool();
	print_small_pool();
	print_large_pool();
	ft_printf("=========%sEnd Heap Print%s=========\n", BLUE, WHITE);

}

void	print_tiny_pool() {
	if (!heap.page_size) {
		return ;
	}
//	ft_printf("===%sStart TINY Pool Print%s===\n", RED, WHITE);
	print_pool_table(&TINY_POOL);
	print_table_union();
	print_pool(&TINY_POOL);
//	ft_printf("====%sEnd TINY Pool Print%s====\n", RED, WHITE);
}



void	print_large_pool() {
	if (!heap.page_size) {
		return ;
	}

//	ft_printf("===%sStart LARGE Pool Print%s===\n", CYAN, WHITE);
	print_pool_table(&LARGE_POOL);
	print_table_union();
	print_pool(&LARGE_POOL);
//	ft_printf("====%sEnd LARGE Pool Print%s====\n", CYAN, WHITE);
}

void	print_small_pool() {
	if (!heap.page_size) {
		return ;
	}
//	ft_printf("===%sStart SMALL Pool Print%s===\n", PURPLE, WHITE);
	print_pool_table(&SMALL_POOL);
	print_table_union();
	print_pool(&SMALL_POOL);
//	ft_printf("====%sEnd SMALL Pool Print%s====\n", PURPLE, WHITE);
}