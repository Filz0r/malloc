/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 11:05:31 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/22 12:27:44 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>

void *allocate_new_pool(t_pool *pool, size_t pool_size) {
	void *pool_memory = mmap(NULL, pool_size, MMAP_FLAGS);
	ft_printf("mmap_addr: %p\n", pool_memory);
//	ft_printf("size: %zu\n", pool_size);
//	ft_printf("TYPE: %s\n", get_heap_type(pool->group));
	if (pool_memory == MAP_FAILED) {
		return NULL;
	}

	pool->data = pool_memory;
	pool->total = pool_size;
	pool->free_bytes = pool_size;
	pool->max_chunk_size = SET_MAX_CHUNK_SIZE(pool->group);
	// Setup first block
	t_block *first_block = (t_block *)pool_memory;
	first_block->size = pool_size - (sizeof(t_block));
	first_block->freed = true;

	pool->chunks = first_block;
	pool->first = first_block;
	pool->last = NULL;
	pool->unmapped = first_block;
	pool->block_end = pool_memory + pool_size;
	heap.total_memory += pool_size;
	return pool_memory;
}

void *allocate_large_block(size_t size) {
	size_t total_size = align_up(sizeof(t_block) + size);
	total_size = align_up_to_power_of_2(total_size, heap.page_size);
	ft_printf("total_size1: %zu\n", total_size);
	void *memory = mmap(NULL, total_size, MMAP_FLAGS);
	if (memory == MAP_FAILED)
		return NULL;

	t_block *block = (t_block *) memory;
	block->size = size;
	block->freed = false;

//	if (LARGE_POOL.chunks)
//		LARGE_POOL.chunks->prev = block;

	LARGE_POOL.chunks = block;
	LARGE_POOL.total += total_size;
	LARGE_POOL.block_count++;
	heap.total_memory += total_size;

	return (void *) ((char *) block + sizeof(t_block));
}

t_block *find_free_block(t_pool *pool, size_t requested_size) {
	size_t aligned = align_up(requested_size);
	t_block *current = pool->chunks;

	while (IS_VALID_BLOCK(pool, current)) {
//		print_chunk(current);
		if (current->freed && current->size >= aligned) {
			return current;
		}
		current = NEXT_BLOCK(current);
	}
//	ft_printf("m0\n");
	return NULL;
}

void *split_block(t_pool *pool, t_block *block, size_t requested_size) {
	size_t aligned_size = align_up(requested_size);

	if (CAN_SPLIT_BLOCK(block, requested_size)) {
		ft_printf("in\n");
		t_block *new_block = NEXT_BLOCK(block);
		new_block->size = block->size - aligned_size - sizeof(t_block);
		new_block->freed = true;

		block->size = aligned_size;
		block->freed = false;
	} else {
		ft_printf("out\n");
		block->freed = false;
	}
	pool->last = block;
	pool->first = block + aligned_size;
	pool->block_count++;

	return BLOCK_DATA(block);
}

void coalesce_free_blocks(t_pool *pool) {
	t_block *current = pool->chunks;

	while (IS_VALID_BLOCK(pool, current)) {
		t_block *next = NEXT_BLOCK(current);

		if (IS_VALID_BLOCK(pool, next) &&
			current->freed && next->freed) {
			// Merge blocks
			current->size += BLOCK_TOTAL_SIZE(next);
		} else {
			current = next;
		}
	}
}

void *pool_is_full(t_pool *pool, size_t requested) {
	ft_printf("og: %s\n", get_heap_type(pool->group));
//	print_pool_table(pool);
	if (pool->group == TINY) {
		t_pool *new_pool = &SMALL_POOL;
		size_t pool_size = SMALL_POOL_SIZE_MULTIPLIER * heap.page_size;
		if (!new_pool->data && !allocate_new_pool(new_pool, pool_size))
			return NULL;
		size_t aligned_size = align_up(requested);
		t_block *new_block = new_pool->first;
		print_chunk(new_block);
		size_t total_size = TOTAL_SIZE(requested);
		ft_printf("new_b %p\n", new_block);
//		print_pool_table(new_pool);
		if (new_block) {
			if (CAN_SPLIT_BLOCK(new_block, total_size)) {
				ft_printf("in\n");
				t_block *rest = SPLIT_BLOCK(new_pool, total_size);
				rest->size = pool->free_bytes - total_size;
				rest->freed = true;
//			ft_bzero(rest->padding, PADDING);
				new_pool->first = rest;
				new_pool->block_count++;
				new_pool->free_bytes -= total_size;
				new_block->size = aligned_size;
				new_block->freed = false;
//			ft_bzero(new_block->padding, PADDING);
				return (void *) new_block + sizeof(t_block);
			}
			ft_printf("hm\n");
		}
		ft_printf("hm1\n");
	}
//	ft_printf("yo4\n");
	ft_printf("hm2\n");
	return allocate_large_block(requested);
}

void *find_next_block(t_pool *pool, size_t size) {
	size_t aligned_size = align_up(size);
	t_block *block = pool->first;
	if (block) {
		if (CAN_SPLIT_BLOCK(block, aligned_size)) {
			t_block *rest = SPLIT_BLOCK(pool, TOTAL_SIZE(size));

			if (DOES_BLOCK_FIT(pool, rest)) {
				return pool_is_full(pool, size);
			}

			rest->size = pool->free_bytes - (aligned_size + (sizeof(t_block)));
			rest->freed = true;
//			ft_bzero(rest->padding, PADDING);
			pool->first = rest;
			pool->block_count++;
			pool->free_bytes -= aligned_size;
			block->size = aligned_size;
			block->freed = false;
//			ft_bzero(block->padding, PADDING);
			return (void *) block + sizeof(t_block);
		}
	}
	return NULL;  // No space available
}
//void	*find_next_block(t_pool *pool, t_sizes alignments) {
//	t_block *current = pool->first;
//	t_block *suitable_block = NULL;
//	while (current) {
//		if (current->freed && current->size >= alignments.total_size) {
//			suitable_block = current;
//			break;
//		}
//		current = NEXT_BLOCK(current);
//	}
//
//	if (!suitable_block) {
//		pthread_mutex_unlock(&lock);
//		return NULL;
//	}
//
//	// Split block if necessary
//	if (suitable_block->size >= alignments.total_size + sizeof(t_block) + ALIGNMENT) {
//		size_t remaining_size = suitable_block->size - alignments.total_size;
//
//		t_block *new_block = (t_block*)((char*)suitable_block + alignments.total_size);
//		new_block->size = remaining_size - sizeof(t_block);
//		new_block->freed = true;
//
////		if (suitable_block->next)
////			suitable_block->next->prev = new_block;
//
////		suitable_block->next = new_block;
//		suitable_block->size = alignments.aligned_size;
//
//		if (pool->last == suitable_block)
//			pool->last = new_block;
//
//		if (pool->first == suitable_block)
//			pool->first = new_block;
//	}
//	suitable_block->freed = false;
//	pool->free_bytes -= alignments.total_size;
//	pool->block_count++;
//
//	void *user_ptr = (void*)((char*)suitable_block + sizeof(t_block));
//	return user_ptr;
//}
