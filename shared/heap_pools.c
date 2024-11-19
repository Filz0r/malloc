/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_pools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 23:15:25 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/17 19:38:45 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>

static void initialize_tiny_pool() {
	TINY_POOL.group = TINY;
	TINY_POOL.total = TINY_POOL_SIZE_MULTIPLIER * heap.page_size;
	TINY_POOL.free_bytes = TINY_POOL.total;
	TINY_POOL.block_count = 0;
	TINY_POOL.data = NULL;
	TINY_POOL.chunks = NULL;
	TINY_POOL.first = NULL;
	TINY_POOL.last = NULL;
}

static void	initialize_small_pool() {
	SMALL_POOL.group = SMALL;
	SMALL_POOL.total = SMALL_POOL_SIZE_MULTIPLIER * heap.page_size;
	SMALL_POOL.free_bytes = SMALL_POOL.total;
	SMALL_POOL.block_count = 0;
	SMALL_POOL.data = NULL;
	SMALL_POOL.chunks = NULL;
	SMALL_POOL.first = NULL;
	SMALL_POOL.last = NULL;
}

static void	initialize_large_pool() {
	LARGE_POOL.group = LARGE;
	LARGE_POOL.total = 0;  // Will grow as needed
	LARGE_POOL.free_bytes = 0;
	LARGE_POOL.block_count = 0;
	LARGE_POOL.data = NULL;
	LARGE_POOL.chunks = NULL;
	LARGE_POOL.first = NULL;
	LARGE_POOL.last = NULL;
}

void pool_initialization(void) {
	if (heap.page_size) {
		return;
	}
	heap.page_size = getpagesize();
	if (getrlimit(RLIMIT_AS, &heap.limits) == -1)
		return;

	ft_bzero(&heap.pools, sizeof(heap.pools));
	ft_bzero(&heap.large, sizeof(heap.large));

	initialize_tiny_pool();
	initialize_small_pool();
	initialize_large_pool();

	heap.total_memory = 0;
}

t_heap_group get_pool_group(size_t size) {
	size_t aligned_size = align_up(size);
	size_t tiny_max = TINY_POOL_CHUNK_MAX_SIZE_MULTIPLIER(TINY_POOL_SIZE_MULTIPLIER * heap.page_size);
	size_t small_max = SMALL_POOL_CHUNK_MAX_SIZE_MULTIPLIER(SMALL_POOL_SIZE_MULTIPLIER * heap.page_size);

	if (aligned_size <= tiny_max)
		return TINY;
	else if (aligned_size <= small_max)
		return SMALL;
	return LARGE;
}

t_pool *get_pool_by_group(t_heap_group group) {
	switch (group) {
		case TINY:
			return &TINY_POOL;
		case SMALL:
			return &SMALL_POOL;
		case LARGE:
			return &LARGE_POOL;
		default:
			return NULL;
	}
}

t_pool *find_pool(size_t size, t_heap_group group) {
	t_pool *pool = get_pool_by_group(group);

	if (!pool) {
		pthread_mutex_unlock(&lock);
		return NULL;
	}

	// Handle large allocations differently
	if (group == LARGE) {
		void *ptr = allocate_large_block(size);
		pthread_mutex_unlock(&lock);
		return ptr;
	}

	// Create pool if it doesn't exist
	if (!pool->data) {
		size_t pool_size;
		if (group == TINY)
			pool_size = TINY_POOL_SIZE_MULTIPLIER * heap.page_size;
		else
			pool_size = SMALL_POOL_SIZE_MULTIPLIER * heap.page_size;

		if (!allocate_new_pool(pool, pool_size)) {
			pthread_mutex_unlock(&lock);
			return NULL;
		}
	}
	return pool;
}