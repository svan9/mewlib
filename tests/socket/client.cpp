#include "mewlib.h"
#include "mewtypes.h"
#include "mewfs.hpp"
#include "mewsocket.hpp"
#include <string>
#include <chrono>
using namespace mew::socket;

void MySocketListener(Client& client, SOCKET rec, const char* user_data) {
	const char* message = user_data;
	size_t message_size = strlen(message);
	printf("Received message: %s\n", message);
}

void MainListener(Client& client, SOCKET rec, const char* user_data) {
	while (!client.IsRunning()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	while (client.IsRunning()) {
		printf("Message: ");
		char buffer[1024] = {0};
		scanf("%1023s", buffer);
		if (strcmp(buffer, "exit") == 0) {
			printf("Exiting client...\n");
			break;
		}
		client.Send(buffer, strlen(buffer));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	// while (client.IsRunning()) {
	// 	std::this_thread::sleep_for(std::chrono::seconds(10));
	// }
}

int main() {
	Client socket;
	socket.AddListener(MySocketListener);
	socket.SetMainThread(MainListener);
	socket.Listen(8080, SOCKET_LOCALHOST);
	return 0;
}