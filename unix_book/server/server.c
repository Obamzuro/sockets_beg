/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 15:46:49 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/07 17:43:56 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>

#define BUFLEN 128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

void	serve(int sockfd)
{
	int		clfd;
	FILE	*fp;
	char	buf[BULEN];

	set_cloexec(sockfd);
}

int		initserver(int type, const struct sockaddr *addr,
		socklen_t alen, int qlen)
{
	int		fd;

	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return (-1);
	if (bind(fd, addr, alen) < 0)
	{
		close(fd);
		return(-1);
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
	{
		if (listen(fd, qlen) < 0)
		{
			close(fd);
			return (-1);
		}
	}
	return (fd);
}

int		main(int argc, char **argv)
{
	struct addrinfo		*ailist;
	struct addrinfo		*aip;
	struct addrinfo		hint;
	int					sockfd;
	int					err;
	int					n;
	char				*host;

	if (argc != 1)
	{
		printf("argc must be 1\n");
		exit(1);
	}
	if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
		n = HOST_NAME_MAX;
	if ((host = malloc(n)) == NULL)
	{
		perror("malloc");
		exit(1);
	}
	if (gethostname(host, n) < 0)
	{
		perror("gethostname");
		exit(1);
	}
	daemonize("ruptimesd");
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((getaddrinfo(host, "ruptime", &hint, &ailist)) != 0)
	{
		perror("getaddrinfo");
		exit(1);
	}
	aip = ailist;
	while (aip != NULL)
	{
		if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0)
		{
			serve(sockfd);
			exit(0);
		}
		aip = aip->next;
	}
	return (0);
}
