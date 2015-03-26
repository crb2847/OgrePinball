#ifndef CODE_NETWORK_H_
#define CODE_NETWORK_H_

#include <cstdint>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

<<<<<<< HEAD
struct NetworkData_t {
=======
struct NetworkIn_t {
	int32_t paddlePos;
};

struct NetworkOut_t {
>>>>>>> cc8ed3762bb35903d6035bcc6082063ada535f32
	int32_t paddlePos;
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
