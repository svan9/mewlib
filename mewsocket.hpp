#ifndef MEW_SOCKET_LIB_SO2U
#define MEW_SOCKET_LIB_SO2U

#include "mewlib.h"
#include "mewtypes.h"
#include "mewstack.hpp"
#include <thread>
#include <vector>
#include <mutex>
#if defined(_WIN32) || defined(_WIN64)
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#pragma comment(lib, "ws2_32.lib")
	typedef SOCKET mew_socket_t;
	#define mew_invalid_socket INVALID_SOCKET
	#define mew_close_socket closesocket
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <fcntl.h>
	typedef int mew_socket_t;
	#define mew_invalid_socket -1
	#define mew_close_socket close
#endif

namespace mew::socket {
	template<u8 SocketType, typename ListenerType>
	class Socket;
	class Client;
	class Server;

	template<u8 SocketType, typename T>
	using SocketListener = void(*)(Socket<SocketType, T>& self, mew_socket_t client, const char* user_data);
	typedef void(*ClientListener)(Client& client, mew_socket_t server, const char* user_data);
	typedef void(*ServerListener)(Server& server, mew_socket_t client, const char* user_data);

	#if defined(_WIN32) || defined(_WIN64)
	inline const char* wsaerrtostr(int err) {
		switch (err) {
			case WSAEINTR: return "Interrupted function call";
			case WSAEACCES: return "Permission denied";
			case WSAEFAULT: return "Bad address";
			case WSAEINVAL: return "Invalid argument";
			case WSAEMFILE: return "Too many open files";
			case WSAEWOULDBLOCK: return "Resource temporarily unavailable";
			case WSAEINPROGRESS: return "Operation now in progress";
			case WSAEALREADY: return "Operation already in progress";
			case WSAENOTSOCK: return "Socket operation on nonsocket";
			case WSAEDESTADDRREQ: return "Destination address required";
			case WSAEMSGSIZE: return "Message too long";
			case WSAEPROTOTYPE: return "Protocol wrong type for socket";
			case WSAENOPROTOOPT: return "Bad protocol option";
			case WSAEPROTONOSUPPORT: return "Protocol not supported";
			case WSAESOCKTNOSUPPORT: return "Socket type not supported";
			case WSAEOPNOTSUPP: return "Operation not supported";
			case WSAEPFNOSUPPORT: return "Protocol family not supported";
			case WSAEAFNOSUPPORT: return "Address family not supported by protocol family";
			case WSAEADDRINUSE: return "Address already in use";
			case WSAEADDRNOTAVAIL: return "Cannot assign requested address";
			case WSAENETDOWN: return "Network is down";
			case WSAENETUNREACH: return "Network is unreachable";
			case WSAENETRESET: return "Network dropped connection on reset";
			case WSAECONNABORTED: return "Software caused connection abort";
			case WSAECONNRESET: return "Connection reset by peer";
			case WSAENOBUFS: return "No buffer space available";
			case WSAEISCONN: return "Socket is already connected";
			case WSAENOTCONN: return "Socket is not connected";
			case WSAESHUTDOWN: return "Cannot send after socket shutdown";
			case WSAETOOMANYREFS: return "Too many references";
			case WSAETIMEDOUT: return "Connection timed out";
			case WSAECONNREFUSED: return "Connection refused";
			case WSAELOOP: return "Cannot translate name";
			case WSAENAMETOOLONG: return "Name too long";
			case WSAEHOSTDOWN: return "Host is down";
			case WSAEHOSTUNREACH: return "No route to host";
			case WSAENOTEMPTY: return "Directory not empty";
			case WSAEPROCLIM: return "Too many processes";
			case WSAEUSERS: return "User quota exceeded";
			case WSAEDQUOT: return "Disk quota exceeded";
			case WSAESTALE: return "Stale file handle reference";
			case WSAEREMOTE: return "Item is remote";
			case WSASYSNOTREADY: return "Network subsystem is unavailable";
			case WSAVERNOTSUPPORTED: return "Winsock.dll version out of range";
			case WSANOTINITIALISED: return "Successful WSAStartup not yet performed";
			case WSAEDISCON: return "Graceful shutdown in progress";
			case WSATYPE_NOT_FOUND: return "Class type not found";
			case WSAHOST_NOT_FOUND: return "Host not found";
			case WSATRY_AGAIN: return "Nonauthoritative host not found";
			case WSANO_RECOVERY: return "This is a nonrecoverable error";
			case WSANO_DATA: return "Valid name, no data record of requested type";
			default: return "Unknown WSA error";
		}
	}
	#endif

	constexpr const char* SOCKET_LOCALHOST = "127.0.0.1";
	
	enum struct SocketStatus : u8 {
		connected,
		err_socket_init,
		err_socket_bind,
		err_socket_connect,
		disconnected,
	};

	template<u8 SocketType, typename ListenerType>
	class Socket {
	private:
		mew_socket_t m_socket_fd;
		SocketStatus m_status;
		u16 m_port;
		char* m_address;
		mew::stack<ListenerType> m_listener;
		mew::stack<mew_socket_t> m_clients;
		std::mutex m_mutex;
		
	public:
		Socket() : m_socket_fd(mew_invalid_socket), m_status(SocketStatus::err_socket_init) {}
		
		bool IsRunning() const {
			return m_status == SocketStatus::connected;
		}

		SocketStatus GetStatus() const {
			return m_status;
		}

		mew_socket_t GetSocketFD() const {
			return m_socket_fd;
		}

		void AddListener(ListenerType listener) {
			m_listener.push(listener);
		}

		virtual void SetMainThread(ListenerType listener) = 0;
		virtual void _ListenerForData(ListenerType listener, Socket<SocketType, ListenerType>* self, mew_socket_t rec, const char* user_data = nullptr) = 0;
		
		void Listen(u16 port, const char* address) {
			m_port = port;
			m_address = scopy(address);

#if defined(_WIN32) || defined(_WIN64)
			WSADATA wsaData;
			auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != NO_ERROR) {
				wprintf(L"Error at WSAStartup()\n");
				return;
			}
#endif
			m_socket_fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_socket_fd == mew_invalid_socket) {
				m_status = SocketStatus::err_socket_init;
				return;
			}

			struct sockaddr_in server_addr;
			server_addr.sin_family = AF_INET;
			server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			server_addr.sin_port = htons(m_port);

			if constexpr(SocketType == 0) {
				if (::bind(m_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
					m_status = SocketStatus::err_socket_bind;
#if defined(_WIN32) || defined(_WIN64)
					printf("Failed to bind socket: %s\n", wsaerrtostr(WSAGetLastError()));
#else
					printf("Failed to bind socket: %s\n", strerror(errno));
#endif
					return;
				}
				if (listen(m_socket_fd, 5) < 0) {
					m_status = SocketStatus::err_socket_connect;
					return;
				}
			} else if constexpr(SocketType == 1) {
				server_addr.sin_addr.s_addr = inet_addr(m_address);
				if (::connect(m_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
					m_status = SocketStatus::err_socket_connect;
#if defined(_WIN32) || defined(_WIN64)
					printf("Failed to connect socket: %s\n", wsaerrtostr(WSAGetLastError()));
#else
					printf("Failed to connect socket: %s\n", strerror(errno));
#endif
					return;
				}
			}

			m_status = SocketStatus::connected;

			const size_t buffer_size = 1024;
			char buffer[buffer_size] = {0};

			while (m_status == SocketStatus::connected) {
				// Listen for incoming connections
				// This is a placeholder for the actual implementation
				// In a real implementation, you would use select() or poll() to wait for events
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if constexpr(SocketType == 0) {
					sockaddr_in clientInfo;
					memset(&clientInfo, 0, sizeof(clientInfo));
					int clientInfo_size = sizeof(clientInfo);

					// Set socket to non-blocking mode (only once, outside the loop in production)

#if defined(_WIN32) || defined(_WIN64)
					u_long mode = 1;
					ioctlsocket(m_socket_fd, FIONBIO, &mode);
#else
					int flags = fcntl(m_socket_fd, F_GETFL, 0);
					fcntl(m_socket_fd, F_SETFL, flags | O_NONBLOCK);
#endif
					// Non-blocking accept: returns immediately if no connection is present
					SOCKET client_socket = accept(m_socket_fd, (sockaddr*)&clientInfo, &clientInfo_size);
					if (client_socket != -1
#if defined(_WIN32) || defined(_WIN64)
						&& client_socket != INVALID_SOCKET
#endif
					) {
						std::lock_guard<std::mutex> lock(m_mutex);
						m_clients.push(client_socket);
						printf("New connection accepted: %d\n", (int)client_socket);
					}

					// Handle data from all clients
					std::lock_guard<std::mutex> lock(m_mutex);
					for (auto& client : m_clients) {
						int resc = Receive(buffer, client, buffer_size);
						if (resc > 0) {
							for (auto& listener : m_listener) {
								_ListenerForData(listener, this, client, buffer);
							}
						}
					}
				}
				else if constexpr(SocketType == 1) {
					// Client mode, we can receive data from the server
					int resc = Receive(buffer, m_socket_fd, buffer_size);
					if (resc > 0) {
						// Notify listeners about the received data
						m_mutex.lock();
						for (auto& listener : m_listener) {
							_ListenerForData(listener, this, m_socket_fd, buffer);
						}
						m_mutex.unlock();
					}
				}
			}
		}

		int Send(const char* data, mew_socket_t fd, size_t size) {
			if (m_status != SocketStatus::connected) return -1;
			return send(fd, data, size, 0);
		}

		int Receive(char* buffer, mew_socket_t fd, size_t size) {
			if (m_status != SocketStatus::connected) return -1;
			return recv(fd, buffer, size, 0);
		}

		~Socket() {
			if (m_socket_fd != mew_invalid_socket) {
				mew_close_socket(m_socket_fd);
			}
			if (m_address) {
				free(m_address);
			}
			m_status = SocketStatus::disconnected;
	#if defined(_WIN32) || defined(_WIN64)
			WSACleanup();
#endif
		}
	};

	class Server: public Socket<0, ServerListener> {
	public:
		Server() : Socket<0, ServerListener>() {}

		void SetMainThread(ServerListener listener) override {
			std::thread mthr(listener, std::ref(*this), GetSocketFD(), nullptr);
			mthr.detach();
		}

		void _ListenerForData(ServerListener listener, Socket<0, ServerListener>* self, mew_socket_t rec, const char* user_data = nullptr) override {
			std::thread thread(listener, std::ref(*this), rec, user_data);
			thread.detach();
		}
	};

	class Client: public Socket<1, ClientListener> {
	public:
		Client() : Socket<1, ClientListener>() {}

		void SetMainThread(ClientListener listener) override {
			std::thread mthr(listener, std::ref(*this), GetSocketFD(), nullptr);
			mthr.detach();
		}

		void _ListenerForData(ClientListener listener, Socket<1, ClientListener>* self, mew_socket_t rec,const char* user_data = nullptr) override {
			std::thread thread(listener, std::ref(*this), rec, user_data);
			thread.detach();
		}

		int Send(const char* data, size_t size) {
			if (GetStatus() != SocketStatus::connected) { MewWarnMessage("Socket was closed"); return -1;}
			return send(GetSocketFD(), data, size, 0);
		}

		int Receive(char* buffer, size_t size) {
			if (GetStatus() != SocketStatus::connected) { MewWarnMessage("Socket was closed"); return -1;}
			return recv(GetSocketFD(), buffer, size, 0);
		}
	};

	
};

#endif