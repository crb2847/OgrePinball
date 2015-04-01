#include "Network.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

Network::Network() {
	lastSend = 0;
	srand (time(NULL));
	rndId = rand();
	peerAddr.sin_family = AF_INET;
	peerAddr.sin_addr.s_addr = INADDR_ANY;
	peerAddr.sin_port = htons(NET_PORT);

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	int broadcast = 1;
	setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	fcntl(sd, F_SETFL, O_NONBLOCK);
	bind(sd, (struct sockaddr *) &peerAddr, sizeof(peerAddr));
}

bool Network::connect(bool *server) {
	NetworkData_t nd_in, nd_out;
	bool res = false;
	while(read(&nd_in)) {
		if (rndId == nd_in.rndId) continue;
		*server = (rndId < nd_in.rndId);
		res = true;
		break;
	}
	if (!res) {
		peerAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
		peerAddr.sin_port = htons(NET_PORT);
	}
	nd_out.type = NET_DISCOVER;
	nd_out.rndId = rndId;
	write(&nd_out, res ? 0 : 3);
	return res;
}

bool Network::read(NetworkData_t *in) {
	unsigned int addrLen = sizeof(peerAddr);
	int n = recvfrom(sd, in, sizeof(NetworkData_t), 0, (struct sockaddr *) &peerAddr, &addrLen);
	return (n == sizeof(NetworkData_t));
}

void Network::write(NetworkData_t *out, int limit){
	if (limit && (clock()-lastSend) < (CLOCKS_PER_SEC/limit)) return;
	out->rndId = rndId;
	sendto(sd, out, sizeof(NetworkData_t), 0, (struct sockaddr *) &peerAddr, sizeof(peerAddr));
	lastSend = clock();
}

Network::~Network() { }
