/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 20:51:37 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/19 13:23:32 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <print_utils.h>

int	ft_type_parser(const char *buffer, int *flag, va_list(args))
{
	int	k;

	k = 0;
	if (*buffer == 'c')
		k += ft_putchar(va_arg(args, int));
	else if (*buffer == 's')
		k += ft_putstr(va_arg(args, char *));
	else if (*buffer == 'd' || *buffer == 'i')
		k += ft_putnbr(va_arg(args, int));
	else if (*buffer == '%')
		k += ft_putchar((int) '%');
	else if (*buffer == 'u')
		k += ft_putu(va_arg(args, unsigned int));
	else if (*buffer == 'x' || *buffer == 'X')
		k += ft_puthex(*buffer, va_arg(args, unsigned int));
	else if (*buffer == 'p')
		k += ft_handle_pointer(*buffer, va_arg(args, unsigned long int));
	else if (*buffer == 'z' && *(buffer + 1) == 'u') {
		k += ft_putulong(va_arg(args, unsigned long));
		*flag = 1;
	}
	return (k);
}

int	ft_handle_pointer(int hex, unsigned long int nb)
{
	int	k;

	k = 0;
	if (nb)
	{
		k += ft_putstr("0x");
		k += ft_puthex(hex, nb);
	}
	else
		k += ft_putstr("(nil)");
	return (k);
}


int	ft_putchar(char c)
{
	return write(1, &c, 1);
}

int	ft_putstr(const char *str) {
	int ret = 0;

	ret += write(1, str, ft_strlen(str));
//	while(str && *str)
//          ret += ft_putchar(*str++);

	return ret;
}

void	ft_putnbr_base(int nb, size_t base)
{
	if (base != 10 && base != 16)
		return;
	long nbr = nb;
	if (nbr < 0)
	{
		nbr = -nbr;
		ft_putchar('-');
	}
	if ((size_t)nbr > base - 1)
	{
		ft_putnbr_base((int)(nbr / base), base);
		ft_putnbr_base((int)(nbr % base), base);
	} else
		ft_putchar(HEX_LOWER[nbr]);
}

int	ft_putnbr(int nb)
{
	int	k;

	k = 0;
	if (nb == -2147483648)
	{
		k += ft_putchar('-');
		k += ft_putchar('2');
		k += ft_putnbr(147483648);
	}
	else if (nb < 0)
	{
		k += ft_putchar('-');
		nb = -nb;
		k += ft_putnbr(nb);
	}
	else if (nb > 9)
	{
		k += ft_putnbr(nb / 10);
		k += ft_putnbr(nb % 10);
	}
	else
	{
		k += ft_putchar(nb + 48);
	}
	return (k);
}

int	ft_putu(unsigned int nb)
{
	int	k;

	k = 0;
	if (nb > 9)
	{
		k += ft_putu(nb / 10);
		k += ft_putu(nb % 10);
	}
	else
		k += ft_putchar(nb + 48);
	return (k);
}

int	ft_putulong(unsigned long nb)
{
	int	k;

	k = 0;
	if (nb > 9)
	{
		k += ft_putulong(nb / 10);
		k += ft_putulong(nb % 10);
	}
	else
		k += ft_putchar(nb + 48);
	return (k);
}

int	ft_puthex(int hex, unsigned long int nb)
{
	int		k;

	k = 0;
	if (nb > 15)
	{
		k += ft_puthex(hex, nb / 16);
		k += ft_puthex(hex, nb % 16);
	}
	else
	{
		if (hex == 'X')
			k += ft_putchar(HEX_UPPER[nb]);
		else
			k += ft_putchar(HEX_LOWER[nb]);
	}
	return (k);
}

int ft_strlen(const char *str) {
	int i = 0;
	while (str && *str++)
		i++;
	return i;
}

int ft_num_len(unsigned long nb) {
	int				i;

	i = 1;
	while (nb > 9)
	{
		nb = nb / 10;
		i++;
	}
	return (i);
}
