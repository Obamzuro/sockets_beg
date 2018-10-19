/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 14:11:40 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/07 15:11:52 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>

#define MAXSLEEP 128
#define BUFLEN 128

int		connect_retry(int domain, int type, int protocol,
		const struct sockaddr *addr, socklen_t alen)
{
	int		numsec;
	int		fd;

	numsec = 1;
	while (numsec <= MAXSLEEP)
	{
		if ((fd = socket(domain, type, protocol)) < 0)
			return (-1);
		if (connect(fd, addr, alen) == 0)
			return (fd);
		close(fd);
		if (numsec <= MAXSLEEP/2)
			sleep(numsec);
		numsec <<= 1;
	}
	return (-1);
}

void	print_uptime(int sockfd)
{
	int		n;
	char	buf[BUFLEN];

	while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
		write(STDOUT_FILENO, buf, n);
	if (n < 0)
		perror("error recv");
}

int		main(int argc, char **argv)
{
	struct addrinfo		*ailist;
	struct addrinfo		*aip;
	struct addrinfo		hint;
	int					sockfd;
	int					err;

	if (argc != 2)
	{
		perror("Usage: ruptime hostname");
		return (-1);
	}
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0)
		perror("getaddrinfo");
	aip = ailist;
	while (aip != NULL)
	{
		if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0,
						aip->ai_addr, aip->ai_addrlen)) < 0)
			err = errno;
		else
		{
			print_uptime(sockfd);
			return (0);
		}
		aip = aip->ai_next;
	}
	perror("can't connect with hostname");
}
