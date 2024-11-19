/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priv_malloc.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:53:11 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/19 22:07:58 by fparreir         ###   ########.fr       */
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

# define false	0
# define true	1
# define TINY_POOL_SIZE_MULTIPLIER 128 // * PAGE_SIZE
# define SMALL_POOL_SIZE_MULTIPLIER 1024 // * PAGE_SIZE
# define TINY_POOL_CHUNK_MAX_SIZE_MULTIPLIER(x) (x / 300)
# define SMALL_POOL_CHUNK_MAX_SIZE_MULTIPLIER(x) (x / 50)
# define ALIGNMENT 16
# define HEAP_POOLS 2
# define TINY_POOL_IDX 0
# define SMALL_POOL_IDX 1
# define MMAP_FLAGS PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0


typedef struct s_pool 	t_pool;
typedef struct s_heap	t_heap;
typedef struct s_block	t_block;
typedef struct s_sizes	t_sizes;
typedef enum heap_group t_heap_group;

enum heap_group {
	TINY,
	SMALL,
	LARGE,
};

struct s_pool {
	void			*data; // Address given by mmap
	void			*unmapped;
	size_t			total;
	size_t			free_bytes;
	size_t			block_count;
	t_block			*chunks; // All the chunks
	t_block			*first; // First free chunk
	t_block			*last; // Last chunk
	t_heap_group	group;
};

struct s_block
{
	size_t	size;
	t_block *prev;
	t_block *next;
	uint8_t freed;
};

struct s_heap {
	t_pool 	pools[HEAP_POOLS];
	t_pool	large;
	struct rlimit limits;
	size_t	page_size;
	size_t	total_memory;
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
void 	pool_initialization(void);

// Wrappers
void	*find_next_block(t_pool *first, t_sizes alignments);

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

// Util functions;
void	ft_bzero(void	*str, size_t length);
void	*ft_memset(void *str, int c, size_t length);

// DEBUG
char	*add_padding(int stop, void *ptr);
char	*get_pool_table_middle_line(int end);
char	*get_pool_table_bottom_line(int end);
char	*get_pool_table_top_line(int end);
char	*get_pool_middle_line(int end, uint8_t trigger);
char	*get_pool_bottom_line(int end);
char	*get_pool_top_line(int end);
char	*convert_size_to_unit(size_t size);
char	*get_heap_type(t_heap_group group);
void	print_heap(void);
void	print_table_union(void);
void	print_pool_table(t_pool *ptr);
void	print_pool(t_block *start);
void	print_tiny_pool(void);
void	print_small_pool(void);
void	print_large_pool(void);
