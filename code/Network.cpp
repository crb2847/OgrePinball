#include "Network.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>

Network::Network(bool server) {
	Network::server = server;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
	clientAddr.sin_port = htons(NET_PORT);
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	fcntl(sd, F_SETFL, O_NONBLOCK);
	if (server) {
		struct sockaddr_in srv_a;
		srv_a.sin_family = AF_INET;
		srv_a.sin_port = htons(NET_PORT);
		srv_a.sin_addr.s_addr = INADDR_ANY;
		bind (sd, (struct sockaddr *) &srv_a, sizeof(srv_a));
	} else {
		int broadcast=1;
		if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
			{ printf("Error setting broadcast option\n"); exit(0); }
	}
}

bool Network::read(NetworkIn_t *in) {
	unsigned int addrLen = sizeof(clientAddr);
	int n = recvfrom(sd, in, sizeof(NetworkIn_t), 0, (struct sockaddr *) &clientAddr, &addrLen);
	return (n == sizeof(NetworkIn_t));
}

void Network::write(NetworkOut_t *out){
	sendto(sd, out, sizeof(NetworkOut_t), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
}

Network::~Network() { }
