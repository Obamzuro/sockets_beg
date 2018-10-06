/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 19:24:19 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/06 22:17:32 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

int	main(void)
{
	register int		s;
	register int		bytes;
	struct sockaddr_in	sa;
	char				buffer[BUFSIZ+1];

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return (1);
	}
	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1025);
	sa.sin_addr.s_addr = htonl((((((127 << 8) | 0) << 8) | 0) << 8) | 1);
	if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0)
	{
		perror("socket");
		close(s);
		return (2);
	}
	while ((bytes = read(s, buffer, BUFSIZ)) > 0)
		write(1, buffer, bytes);
	close(s);
	return (0);
}
