/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 09:51:44 by fparreir          #+#    #+#             */
/*   Updated: 2024/11/17 09:53:25 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <priv_malloc.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
t_heap heap = {0};
