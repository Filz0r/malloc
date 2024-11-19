/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:54:05 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/17 11:25:04 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <malloc.h>
#include <priv_malloc.h>
#include <ft_printf.h>

//void	free(void *ptr) {
//	pthread_mutex_lock(&lock);
//	ft_printf("free: |%p|\n", ptr);
//	pthread_mutex_unlock(&lock);
//}

void free(void *ptr) {
	if (!ptr)
		return;

	pthread_mutex_lock(&lock);

	t_block *block = (t_block*)((char*)ptr - sizeof(t_block));

	// Find which pool this block belongs to
	t_pool *pool = NULL;
	if ((void*)block >= TINY_POOL.data &&
		(void*)block < (void*)((char*)TINY_POOL.data + TINY_POOL.total))
		pool = &TINY_POOL;
	else if ((void*)block >= SMALL_POOL.data &&
			 (void*)block < (void*)((char*)SMALL_POOL.data + SMALL_POOL.total))
		pool = &SMALL_POOL;
	else {
		// Check if it's a large allocation
		t_block *large_block = LARGE_POOL.chunks;
		while (large_block) {
			if (large_block == block) {
				// Unmap large allocation
				size_t total_size = align_up_to_power_of_2(
					sizeof(t_block) + block->size,
					heap.page_size
				);

				if (block->prev)
					block->prev->next = block->next;
				if (block->next)
					block->next->prev = block->prev;
				if (LARGE_POOL.chunks == block)
					LARGE_POOL.chunks = block->next;

				LARGE_POOL.total -= total_size;
				LARGE_POOL.block_count--;

				munmap(block, total_size);
				pthread_mutex_unlock(&lock);
				return;
			}
			large_block = large_block->next;
		}
		pthread_mutex_unlock(&lock);
		return;  // Not in our heap
	}

	block->freed = true;

	// Coalescing for TINY and SMALL pools
	if (block->next && block->next->freed) {
		t_block *next_block = block->next;
		block->size += sizeof(t_block) + next_block->size;
		block->next = next_block->next;
		if (next_block->next)
			next_block->next->prev = block;
		if (pool->last == next_block)
			pool->last = block;
		if (pool->first == next_block)
			pool->first = block;
	}

	if (block->prev && block->prev->freed) {
		t_block *prev_block = block->prev;
		prev_block->size += sizeof(t_block) + block->size;
		prev_block->next = block->next;
		if (block->next)
			block->next->prev = prev_block;
		if (pool->last == block)
			pool->last = prev_block;
		block = prev_block;
	}

	if (!pool->first || block < pool->first)
		pool->first = block;

	pool->free_bytes += block->size;
	pool->block_count--;

	pthread_mutex_unlock(&lock);
}