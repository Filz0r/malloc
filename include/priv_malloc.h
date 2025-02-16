/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priv_malloc.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:53:11 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/22 10:19:56 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
// System
# include <unistd.h>
# include <stddef.h>
# include <pthread.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stdint.h>

// Mine
#include <ft_printf.h>

# define false    0
# define true    1
# define PADDING 7
# define HEAP_POOLS 2
# define TINY_POOL_IDX 0
# define SMALL_POOL_IDX 1
# define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0
# define TINY_POOL_SIZE_MULTIPLIER 128 // * PAGE_SIZE
# define SMALL_POOL_SIZE_MULTIPLIER 1024 // * PAGE_SIZE
# define TINY_POOL_CHUNK_MAX_SIZE_MULTIPLIER(x) (x / 300)
# define SMALL_POOL_CHUNK_MAX_SIZE_MULTIPLIER(x) (x / 50)
# define SET_MAX_CHUNK_SIZE(group) \
(group == TINY ? TINY_POOL_CHUNK_MAX_SIZE_MULTIPLIER(TINY_POOL_SIZE_MULTIPLIER * heap.page_size) : SMALL_POOL_CHUNK_MAX_SIZE_MULTIPLIER(SMALL_POOL_SIZE_MULTIPLIER * heap.page_size))

# define TOTAL_SIZE(requested_size) (sizeof(t_block) + align_up(requested_size))

# define DOES_BLOCK_FIT(pool, block) (block >= ((t_block *)(char *)pool->block_end))

# define ALIGNMENT 16


typedef struct s_pool t_pool;
typedef struct s_large_pool t_large_pool;
typedef struct s_large_block t_large_block;
typedef struct s_heap t_heap;
typedef struct s_block t_block;
typedef struct s_sizes t_sizes;
typedef enum heap_group t_heap_group;

enum heap_group {
	TINY,
	SMALL,
	LARGE,
};

struct s_pool {
	void *data; // Address given by mmap aka start of the pool;
	void *unmapped;
	void *block_end; //Final address of the block;
	size_t total;
	size_t free_bytes;
	size_t block_count;
	size_t max_chunk_size;
	t_block *chunks; // Start of the allocated memory
	t_block *first; // First free chunk
	t_block *last; // Last chunk TODO: Delete this?
	t_heap_group group;
};

struct s_large_pool {
	size_t total;
	size_t block_count;
	t_large_block *first; // Start of the block list
	t_large_block *last; // End of the block list
	t_heap_group group;
};

struct s_block {
	size_t size;
	uint8_t freed;
	uint8_t padding[7]; // For alignment purposes if needed
};

struct s_large_block {
	size_t size;
	uint8_t free;
	t_large_block *next;
	t_large_block *prev;
};

struct s_heap {
	t_pool pools[HEAP_POOLS];
	t_pool large;
	struct rlimit limits;
	size_t page_size;
	size_t total_memory;
};

struct s_sizes {
	size_t total_size;
	size_t aligned_size;
};


extern t_heap heap;
extern pthread_mutex_t lock;

#define TINY_POOL (heap.pools[TINY_POOL_IDX])
#define SMALL_POOL (heap.pools[SMALL_POOL_IDX])
#define LARGE_POOL (heap.large)

// Memory pool initialization;
void pool_initialization(void);

// Wrappers
void *find_next_block(t_pool *first, size_t requested_size);

void *pool_is_full(t_pool *pool, size_t requested_size);

// Allocations
t_heap_group get_pool_group(size_t size);

t_pool *get_pool_by_group(t_heap_group group);

void *allocate_new_pool(t_pool *pool, size_t pool_size);

void *allocate_large_block(size_t size);


// Pool management
t_pool *find_pool(size_t size, t_heap_group group);

// Alignments
static inline size_t align_up_to_power_of_2(size_t size, size_t power) {
	return (size + (power - 1)) & ~(power - 1);
}

static inline size_t align_down_to_power_of_2(size_t size, size_t power) {
	return size & ~(power - 1);
}

static inline size_t align_up(size_t size) {
	return align_up_to_power_of_2(size, ALIGNMENT);
}

static inline size_t align_down(size_t size) {
	return align_down_to_power_of_2(size, ALIGNMENT);
}

// Get start of block's data area (after header)
#define BLOCK_DATA(b) ((void*)((char*)(b) + sizeof(t_block)))

// Get block from data pointer (useful for free())
#define BLOCK_FROM_DATA(ptr) ((t_block*)((char*)(ptr) - sizeof(t_block)))

// Get next block, accounting for alignment
#define NEXT_BLOCK(b) ((t_block*)((char*)BLOCK_DATA(b) + (b)->size))

// Get total size of block including header and alignment
#define BLOCK_TOTAL_SIZE(b) (sizeof(t_block) + align_up((b)->size))

// Check if block is within pool bounds
#define IS_VALID_BLOCK(pool, block) \
    ((void*)(block) >= (pool)->data && (void*)(block) < (pool)->block_end)

// Get block size needed for requested size (including header and alignment)
#define REQUEST_TO_BLOCK_SIZE(size) (align_up(size) + sizeof(t_block))

// Check if block can be split (ensures minimum size for both parts)
#define CAN_SPLIT_BLOCK(block, requested_size) \
    ((block)->size >= (align_up(requested_size) + sizeof(t_block) + ALIGNMENT))

#define SPLIT_BLOCK(pool, total) ((t_block  *)((char *)pool->first + (total)))

// Util functions;
void ft_bzero(void *str, size_t length);

void *ft_memset(void *str, int c, size_t length);

// DEBUG
char *add_padding(int stop, void *ptr);

char *get_pool_table_middle_line(int end);

char *get_pool_table_bottom_line(int end);

char *get_pool_table_top_line(int end);

char *get_pool_middle_line(int end, uint8_t trigger);

char *get_pool_bottom_line(int end);

char *get_pool_top_line(int end);

char *convert_size_to_unit(size_t size);

char *get_heap_type(t_heap_group group);

void print_heap(void);

void print_chunk(t_block *chunk);

void print_table_union(void);

void print_pool_table(t_pool *ptr);

void print_pool(t_pool *pool);

void print_tiny_pool(void);

void print_small_pool(void);

void print_large_pool(void);
