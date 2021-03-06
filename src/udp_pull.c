#include "udp_pull.h"

void* udp_pull(void* leflux)
{
    struct flux * flux = (struct flux *) leflux;
	struct epoll_event ev;
	memset(&ev, 0, sizeof(struct epoll_event));
	int epollfd;

	epollfd = epoll_create(10);
	FAIL(epollfd);

	struct tabClients tabClientsUDP;
	tabClientsUDP.clients =
		(struct sockClient *)malloc(BASE_CLIENTS*sizeof(struct sockClient));
	memset(tabClientsUDP.clients, 0, sizeof(struct sockClient)*BASE_CLIENTS);
	tabClientsUDP.nbClients = 0;

	int baseCouranteUDP = BASE_CLIENTS;

	struct epoll_event events[MAX_EVENTS];

	flux->sock = createSockEventUDP(epollfd, flux->port);
	int sockData = socket(AF_INET, SOCK_DGRAM, 0);

	int nfds;
	int done = 0;

	while(done == 0) //Boucle principale
	{

		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		FAIL(nfds);

		int n;

		for (n = 0; n < nfds; ++n)
		{
			if (events[n].events == EPOLLIN )
			{					
				char * buffer = (char *)malloc(512*sizeof(char));
				memset(buffer, '\0', 512);

				struct sockaddr_in faddr;
				memset(&faddr, 0, sizeof(struct sockaddr_in));
				socklen_t len = sizeof(faddr);

				FAIL(recvfrom(flux->sock, buffer, 512*sizeof(char),0, (struct sockaddr*)&faddr, &len));

				int j = 0;
				int trouve = -1;
				while((trouve == -1) && (j < tabClientsUDP.nbClients))
				{
					struct sockaddr_in * comp = &tabClientsUDP.clients[j].videoClient.orig_addr;
					if(faddr.sin_addr.s_addr == comp->sin_addr.s_addr 
							&& faddr.sin_family == comp->sin_family
							&& faddr.sin_port == comp->sin_port)
					{
						trouve = j;
					}
					j++;
				}
				if(trouve == -1)
				{
					if (tabClientsUDP.nbClients >= baseCouranteUDP)
					{
						baseCouranteUDP *=2;
						struct sockClient *  temp;
						temp = (struct sockClient *) realloc(tabClientsUDP.clients,
								baseCouranteUDP*sizeof(struct sockClient));
						if (temp!=NULL) 
						{
							tabClientsUDP.clients = temp;
						}
						else 
						{
							free (tabClientsUDP.clients);
							puts ("Error (re)allocating memory");
							exit (1);
						}
					}
					initReq(&(tabClientsUDP.clients[tabClientsUDP.nbClients].requete));
					memset(&tabClientsUDP.clients[tabClientsUDP.nbClients].videoClient.orig_addr,0,sizeof(struct sockaddr_in));
					memcpy(&tabClientsUDP.clients[tabClientsUDP.nbClients].videoClient.orig_addr, &faddr, sizeof(struct sockaddr_in));
					tabClientsUDP.clients[tabClientsUDP.nbClients].videoClient.clientSocket = sockData;
					
					tabClientsUDP.clients[tabClientsUDP.nbClients].videoClient.infosVideo = &flux->infosVideo;
					trouve = tabClientsUDP.nbClients++;
				}
				traiteChaine(buffer, &tabClientsUDP.clients[trouve].requete, &tabClientsUDP.clients[trouve].videoClient, 
						epollfd, 0);
			}

		}


	}
	return NULL;
}

