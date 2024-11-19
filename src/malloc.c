/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:14:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/18 14:29:56 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <priv_malloc.h>


static void	debug_alloc(size_t size, t_heap_group group, t_pool *pool, void *ptr) {
	size_t aligned_size = align_up(size);
	size_t total_size = align_up(sizeof(t_block) + aligned_size);


	ft_printf("\n###%sDEBUG%s###\n", GREEN, WHITE);

	ft_printf("REQUESTED SIZE: %d\n", size);
	ft_printf("ALIGNED SIZE: %d\n", aligned_size);
	ft_printf("TOTAL SIZE: %d\n", total_size);

	ft_printf("pool_group for call: %s\n", get_heap_type(group));
	ft_printf("pool_group_addr: %p\n", pool);
	ft_printf("given_addr: %p\n", ptr);
	ft_printf("next_free_addr: %p\n", ptr + total_size);
	ft_printf("first_addr: %p\n", pool->first);
	ft_printf("###%sEND DEBUG%s###\n\n", GREEN, WHITE);

}

void *malloc(size_t size) {
	if (size == 0)
		return NULL;

	pthread_mutex_lock(&lock);
	pool_initialization();
	t_heap_group group = get_pool_group(size);
	// Calculate sizes
	size_t aligned_size = align_up(size);
	size_t total_size = align_up(sizeof(t_block) + aligned_size);
	t_pool *pool = find_pool(size, group);
	void *user_ptr = find_next_block(pool, (t_sizes){total_size, aligned_size});
//	debug_alloc(size, group, pool, user_ptr);
	pthread_mutex_unlock(&lock);
	return user_ptr;
}

//void	*malloc(size_t size) {
//	pthread_mutex_lock(&lock);
//	pool_initialization();
//	ft_printf("malloc: |%u|\n", size);
//	ft_printf("page_size: |%d|\n", heap.page_size);
//	ft_printf("TINY_page_size: |%d|\n", TINY_POOL.total);
//	pthread_mutex_unlock(&lock);
//	return (0x0);
//}