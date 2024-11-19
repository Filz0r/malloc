/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:43:07 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/18 23:30:38 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H


# include <stdarg.h>
# include <unistd.h>

static char *BORDER_MATRIX[] = {
	"╔", "╦", "╗",
	"╠", "╬", "╣",
	"╚", "╩", "╝",
	"═", "║", " ",
};

# define HEX_LOWER "0123456789abcdef"
# define HEX_UPPER "0123456789ABCDEF"

void	ft_putnbr_base(int , size_t);
int		ft_type_parser(const char *buffer, int *flag, va_list(args));
int		ft_handle_pointer(int hex, unsigned long int nb);
int		ft_putstr(const char *str);
int		ft_putchar(char c);
int		ft_putnbr(int c);
int		ft_putu(unsigned int nb);
int		ft_putulong(unsigned long nb);
int		ft_puthex(int hex, unsigned long int nb);
int		ft_strlen(const char *);
int 	ft_num_len(unsigned long nb);

#endif //PRINT_UTILS_H


