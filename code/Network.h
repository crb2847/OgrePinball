#ifndef CODE_NETWORK_H_
#define CODE_NETWORK_H_

#include <cstdint>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

struct NetworkData_t {
	float paddle1Pos;
	float paddle2Pos;
	uint32_t score1;
	uint32_t score2;
	float ballX, ballY; 
};

class Network {
public:
	Network(bool server);
	virtual ~Network();
	bool read(NetworkData_t *in);
	void write(NetworkData_t *out);
private:
	struct sockaddr_in clientAddr;
	bool server;
	int sd;
	const int NET_PORT = 43543;
};

#endif /* CODE_NETWORK_H_ */
