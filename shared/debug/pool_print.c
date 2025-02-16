/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:22:15 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/20 15:38:26 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>
#include <print_utils.h>

void	print_pool_table(t_pool *pool) {

	char *type = get_heap_type(pool->group);
	ft_printf("%s\n", get_pool_top_line(162));
	ft_printf("%s Data addr: %p%s %s ",
			  BORDER_MATRIX[10],
			  pool->data,
			  add_padding(9, pool->data),
			  BORDER_MATRIX[10]);
	ft_printf("Group: %s %s%s\n",
			  type,
			  add_padding(ft_strlen(type) == 4 ? 11 : 10, 0),
			  BORDER_MATRIX[10]);

	ft_printf("%s\n", get_pool_middle_line(162, false));

	ft_printf("%s Chunk ptr: %p%s %s ",
			  BORDER_MATRIX[10],
			  pool->chunks,
			  add_padding(9, pool->chunks),
			  BORDER_MATRIX[10]);
	ft_printf("Pg Size: %zu %s %s\n",
			  pool->total,
			  add_padding(12 - ft_num_len(pool->total), NULL),
			  BORDER_MATRIX[10]);

	ft_printf("%s\n", get_pool_middle_line(162, false));

	ft_printf("%s 1st Chunk: %p%s %s ",
			  BORDER_MATRIX[10],
			  pool->first,
			  add_padding(9, pool->first),
			  BORDER_MATRIX[10]);
	ft_printf("Free Bytes: %zu%s%s\n",
			  pool->free_bytes,
			  add_padding(11 - ft_num_len(pool->free_bytes), 0),
			  BORDER_MATRIX[10]);

	ft_printf("%s\n", get_pool_middle_line(162, false));

	ft_printf("%s Last: %s%p",
			  BORDER_MATRIX[10],
			  add_padding(5, NULL),
			  pool->last);
 	ft_printf("%s %s",
			  add_padding(9, pool->last),
			  BORDER_MATRIX[10]);
	ft_printf(" Allocated Blocks: %zu%s%s\n",
			  pool->block_count,
			  add_padding(5 - ft_num_len(pool->block_count), 0),
			  BORDER_MATRIX[10]);

	ft_printf("%s\n", get_pool_middle_line(162, true));

	ft_printf("%s Unmapped: %s%p",
			  BORDER_MATRIX[10],
			  add_padding(1, 0),
			  pool->block_end);
	ft_printf("%s %s\n",
			  add_padding(pool->block_end ? 25 : 34, 0),
			  BORDER_MATRIX[10]);
	ft_printf("%s\n", get_pool_bottom_line(162));
}

