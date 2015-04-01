#ifndef CODE_NETWORK_H_
#define CODE_NETWORK_H_

#include <cstdint>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

enum PacketType_t {NET_UPDATE, NET_DISCOVER};

struct NetworkData_t {
	uint32_t type;
	uint32_t rndId;
	float paddle1Pos;
	float paddle2Pos;
	uint32_t score1;
	uint32_t score2;
	float ballX, ballY; 
};

class Network {
public:
	Network(void);
	virtual ~Network();
	bool read(NetworkData_t *in);
	void write(NetworkData_t *out, int limit = 120);
	bool connect(bool *server);
private:
	struct sockaddr_in peerAddr;
	int sd, rndId;
	const int NET_PORT = 43543;
	uint64_t lastSend;
};

#endif /* CODE_NETWORK_H_ */
