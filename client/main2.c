/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 19:24:19 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/08 16:23:45 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>

int	main(int argc, char **argv)
{
	register int		s;
	register int		bytes;
	struct sockaddr_in	sa;
	struct hostent		*he;
	char				buf[BUFSIZ + 1];
	char				*host;

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return (1);
	}
	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	if (argc > 1)
	{
		host = argv[1];
		sa.sin_port = htons(1025);
	}
	else
	{
		host = "time.nist.gov";
		sa.sin_port = htons(13);
	}
	if ((he = gethostbyname(host)) == NULL)
	{
		herror(host);
		return (2);
	}
	memcpy(&sa.sin_addr, he->h_addr_list[0], he->h_length);
	if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0)
	{
		perror("connect");
		return (3);
	}
	while ((bytes = read(s, buf, BUFSIZ)) > 0)
		write(1, buf, bytes);
	close(s);
	return (0);
}
