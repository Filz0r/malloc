/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 11:05:31 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/19 15:36:27 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>

void *allocate_new_pool(t_pool *pool, size_t pool_size) {
	void *pool_memory = mmap(NULL, pool_size, MMAP_FLAGS);
	ft_printf("mmap_addr: %p\n", pool_memory);
	if (pool_memory == MAP_FAILED)
		return NULL;

	pool->data = pool_memory;
	pool->total = pool_size;
	pool->free_bytes = pool_size - sizeof(t_block);

	// Setup first block
	t_block *first_block = (t_block*)pool_memory;
	first_block->size = pool_size - sizeof(t_block);
	first_block->freed = true;
	first_block->prev = NULL;
	first_block->next = NULL;

	pool->chunks = first_block;
	pool->first = first_block;
	pool->last = first_block;
	heap.total_memory = pool_size;
	return pool_memory;
}

void *allocate_large_block(size_t size) {
	size_t total_size = align_up(sizeof(t_block) + size);
	total_size = align_up_to_power_of_2(total_size, heap.page_size);

	void *memory = mmap(NULL, total_size, MMAP_FLAGS);
	if (memory == MAP_FAILED)
		return NULL;

	t_block *block = (t_block*)memory;
	block->size = size;
	block->freed = false;
	block->prev = NULL;
	block->next = LARGE_POOL.chunks;

	if (LARGE_POOL.chunks)
		LARGE_POOL.chunks->prev = block;

	LARGE_POOL.chunks = block;
	LARGE_POOL.total += total_size;
	LARGE_POOL.block_count++;
	heap.total_memory += total_size;

	return (void*)((char*)block + sizeof(t_block));
}


void	*find_next_block(t_pool *pool, t_sizes alignments) {


	// Find suitable block
	t_block *current = pool->first;
	t_block *suitable_block = NULL;
	while (current) {
		if (current->freed && current->size >= alignments.total_size) {
			suitable_block = current;
			break;
		}
		current = current->next;
	}

	if (!suitable_block) {
		pthread_mutex_unlock(&lock);
		return NULL;
	}

	// Split block if necessary
	if (suitable_block->size >= alignments.total_size + sizeof(t_block) + ALIGNMENT) {
		size_t remaining_size = suitable_block->size - alignments.total_size;

		t_block *new_block = (t_block*)((char*)suitable_block + alignments.total_size);
		new_block->size = remaining_size - sizeof(t_block);
		new_block->freed = true;
		new_block->prev = suitable_block;
		new_block->next = suitable_block->next;

		if (suitable_block->next)
			suitable_block->next->prev = new_block;

		suitable_block->next = new_block;
		suitable_block->size = alignments.aligned_size;

		if (pool->last == suitable_block)
			pool->last = new_block;

		if (pool->first == suitable_block)
			pool->first = new_block;
	}
	suitable_block->freed = false;
	pool->free_bytes -= alignments.total_size;
	pool->block_count++;

	void *user_ptr = (void*)((char*)suitable_block + sizeof(t_block));
	return user_ptr;
}
