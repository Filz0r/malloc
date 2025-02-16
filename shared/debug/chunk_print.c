/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:16:42 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/21 14:22:33 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>
#include <print_utils.h>

void print_chunk(t_block *chunk) {
//	if (!chunk)
//		return ;
//	ft_printf("%s|%s\n", YELLOW, WHITE);
	ft_putstr(BORDER_MATRIX[10]);
	ft_printf("%s Addr:%s %p %s", YELLOW, WHITE, chunk, BORDER_MATRIX[10]);
	ft_printf(" %sSize:%s %s %s", YELLOW, WHITE, convert_size_to_unit(chunk->size), BORDER_MATRIX[10]);
	ft_printf(" %sFree:%s %s %s\n", YELLOW, WHITE, (chunk->freed ? "true " : "false"), BORDER_MATRIX[10]);
//	ft_printf(" %sNext:%s %p\n", YELLOW, WHITE, chunk->next);
}


void print_pool(t_pool *pool) {
	t_block *temp = pool->chunks;

	ft_printf("%s\n", get_pool_table_top_line(162));
	while (IS_VALID_BLOCK(pool, temp)) {
		t_block *next = NEXT_BLOCK(temp);
		if (temp->size == 0) {
			temp = next;
			continue;
		}
		print_chunk(temp);
//		if (NEXT_BLOCK(next))
//			ft_printf("%s\n", get_pool_table_middle_line(162));
		temp = next;
	}
	ft_printf("%s\n", get_pool_table_bottom_line(162));
}
