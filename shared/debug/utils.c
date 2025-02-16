/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:12:13 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/20 11:51:14 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>
#include <print_utils.h>

// TODO: This file has a LOT of static memory,
//  maybe you should use macros when possible, you idiot

char	*convert_size_to_unit(size_t size) {
	static char buffer[8];
	const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB"};
	size_t unit_index = 0;
	double display_size = (double)size;

	while (display_size >= 1024 && unit_index < 4) {
		display_size /= 1024;
		unit_index++;
	}

	int int_part = (int)display_size;
	int frac_part = (int)((display_size - int_part) * 10);
	ft_bzero(buffer, 8);

	int pos = 0;
	if (int_part < 10) {
		buffer[pos++] = '0' + int_part;
		buffer[pos++] = '.';
		buffer[pos++] = '0' + frac_part;
	} else if (int_part < 100) {
		buffer[pos++] = '0' + (int_part / 10);
		buffer[pos++] = '0' + (int_part % 10);
	} else {
		buffer[pos++] = '0' + (int_part / 100);
		buffer[pos++] = '0' + ((int_part / 10) % 10);
		buffer[pos++] = '0' + (int_part % 10);
	}
//	buffer[pos++] = ' ';
	for (int i = 0; units[unit_index][i] != '\0'; i++) {
		buffer[pos++] = units[unit_index][i];
	}
	while (pos < 7)
		buffer[pos++] = ' ';
	buffer[pos] = '\0';
	return buffer;
}

char *get_pool_table_middle_line(int end) {
	static char middle[256];
	int k = 0, mid, right;

	mid = (end / 2) - 12;
	right = end - 3;

	if (ft_strlen(middle) != 0)
		return middle;
	while(k < end) {
		int i = 0;
		if (k == 0) {
			while (BORDER_MATRIX[3][i])
				middle[k++] = BORDER_MATRIX[3][i++];
		} else if (k == mid || k == mid + 48) {
			while (BORDER_MATRIX[4][i])
				middle[k++] = BORDER_MATRIX[4][i++];
		} else if (k >= right) {
			while (BORDER_MATRIX[5][i])
				middle[k++] = BORDER_MATRIX[5][i++];
		} else if (k != mid) {
			while (BORDER_MATRIX[9][i])
				middle[k++] = BORDER_MATRIX[9][i++];
		}
	}
	return middle;
}

char *get_pool_table_bottom_line(int end) {
	int k;
	static char bottom[256] = {0};

	k = 0;
	int mid, right;

	mid = (end / 2) - 12;
	right = end - 3;

	if (ft_strlen(bottom) != 0)
		return bottom;
	while (k < end) {
		int z = 0;
		if (k == 0) {
			while (BORDER_MATRIX[6][z])
				bottom[k++] = BORDER_MATRIX[6][z++];
		} else if (k == mid || k == mid + 48) {
			while (BORDER_MATRIX[7][z])
				bottom[k++] = BORDER_MATRIX[7][z++];
		}
		else if (k >= right) {
			while (BORDER_MATRIX[8][z])
				bottom[k++] = BORDER_MATRIX[8][z++];
			break ;
		}
		else {
			while (BORDER_MATRIX[9][z])
				bottom[k++] = BORDER_MATRIX[9][z++];
		}
	}
	bottom[k] = '\0';
	return (bottom);
}

char *get_pool_table_top_line(int end) {
	static char top[256] = {0};
	int k = 0, mid, right;

	mid = (end / 2) - 12;
	right = end - 3;
	while (k < end) {
		int z = 0;
		if (k == 0) {
			while (BORDER_MATRIX[3][z])
				top[k++] = BORDER_MATRIX[3][z++];
		} else if (k == mid || k == mid + 48) {
			while (BORDER_MATRIX[1][z])
				top[k++] = BORDER_MATRIX[1][z++];
		}
		else if (k >= right) {
			while (BORDER_MATRIX[5][z])
				top[k++] = BORDER_MATRIX[5][z++];
			break ;
		}
		else {
			while (BORDER_MATRIX[9][z])
				top[k++] = BORDER_MATRIX[9][z++];
		}
	}
	top[k] = '\0';
	return top;
}

char *get_pool_middle_line(int end, uint8_t trigger) {
	static char middle[256];
	int k = 0, mid, right;

	mid = (end / 2) + 3;
	right = end - 3;

	while(k < end) {
		int i = 0;
		if (k == 0) {
			while (BORDER_MATRIX[3][i])
				middle[k++] = BORDER_MATRIX[3][i++];
		} else if (k == mid) {
			int index = trigger == false ? 4 : 7;
			while (BORDER_MATRIX[index][i])
				middle[k++] = BORDER_MATRIX[index][i++];
		} else if (k >= right) {
			while (BORDER_MATRIX[5][i])
				middle[k++] = BORDER_MATRIX[5][i++];
		} else if (k != mid) {
			while (BORDER_MATRIX[9][i])
				middle[k++] = BORDER_MATRIX[9][i++];
		}
	}
	return middle;
}

char *get_pool_top_line(int end) {
	static char top[256];
	int k = 0, mid, right;

	mid = (end / 2) + 3;
	right = end - 3;

	while(k < end) {
		int i = 0;
		if (k == 0) {
			while (BORDER_MATRIX[0][i])
				top[k++] = BORDER_MATRIX[0][i++];
		} else if (k == mid) {
			while (BORDER_MATRIX[1][i])
				top[k++] = BORDER_MATRIX[1][i++];
		} else if (k >= right) {
			while (BORDER_MATRIX[2][i])
				top[k++] = BORDER_MATRIX[2][i++];
		} else if (k != mid) {
			while (BORDER_MATRIX[9][i])
				top[k++] = BORDER_MATRIX[9][i++];
		}
	}
	return top;
}

char *get_pool_bottom_line(int end) {
	static char bottom[256];
	int k = 0,right;

	right = end - 3; // Three is the magic number that, usually, these
					// wierd as shit strings have of length when in memory
					// Aka you are only able to write in multiples of 3 yay math

	while(k < end) {
		int i = 0;
		if (k == 0) {
			while (BORDER_MATRIX[3][i])
				bottom[k++] = BORDER_MATRIX[3][i++];
		} else if (k >= right) {
			while (BORDER_MATRIX[5][i])
				bottom[k++] = BORDER_MATRIX[5][i++];
		} else {
			while (BORDER_MATRIX[9][i])
				bottom[k++] = BORDER_MATRIX[9][i++];
		}
	}
	return bottom;
}

char *add_padding(int stop, void *ptr) {
	static char padding_buffer[65] = {0};

	ft_bzero(padding_buffer, 65);
	if (!ptr) {
		ft_memset(padding_buffer, ' ', stop);
	}
	return padding_buffer;
}

char *get_heap_type(t_heap_group group) {
	switch (group) {
		case TINY:
			return "TINY";
		case SMALL:
			return "SMALL";
		case LARGE:
			return "LARGE";
	}
}

void	print_table_union(void) {
	ft_printf("%s%s%sPOOL", BORDER_MATRIX[10], add_padding(23, 0), YELLOW);
	ft_printf("%s%s%s\n", WHITE, add_padding(25, 0), BORDER_MATRIX[10]);
}