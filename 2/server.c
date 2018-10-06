/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 21:59:37 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/06 22:12:23 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

#define BACKLOG 4

int		main(void)
{
	register int		s;
	register int		c;
	socklen_t			b;
	struct sockaddr_in	sa;
	time_t				t;
	FILE				*client;
	struct tm			*tm;

	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return (1);
	}
	bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1025);
	if (INADDR_ANY)
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) < 0)
	{
		perror("bind");
		return (2);
	}
	switch (fork())
	{
		case -1:
			perror("fork");
			return (3);
			break ;
		case 0:
			break ;
		default:
			close(s);
			return (0);
			break ;
	}
	listen(s, BACKLOG);
	while (1)
	{
		b = sizeof(sa);
		if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0)
		{
			perror("daytimed accept");
			return (4);
		}
		if ((client = fdopen(c, "w")) == NULL)
		{
			perror("daytimed fdopen");
			return (5);
		}
		if ((t = time(NULL)) < 0)
		{
			perror("daytimed time");
			return (6);
		}
		tm = gmtime(&t);
		fprintf(client, "%.4i-%.2i-%.2iT%.2i:%.2i:%.2iZ\n",
            tm->tm_year + 1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);
		fclose(client);
	}
}
