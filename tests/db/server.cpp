#include "mewlib.h"
#include "mewtypes.h"
#include "mewfs.hpp"
#include "mewsocket.hpp"
#include <string>

using namespace mew::socket;

void MySocketListener(Server& server, mew_socket_t client, const char* user_data) {
	const char* message = user_data;
	size_t message_size = strlen(message);
	printf("Received message: %s\n", message);
	server.Send(message, client, message_size);
}

int main() {
	Server socket;
	socket.AddListener(MySocketListener);
	socket.Listen(8080, SOCKET_LOCALHOST);
	return 0;
}