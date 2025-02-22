#pragma once

#ifdef _WIN32 
	#include <winsock2.h>
	#include <windows.h>
#else
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <fcntl.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif

SOCKET udp_create_socket();
void udp_close_socket(SOCKET sock);
int udp_shutdown_socket(SOCKET sock);

int udp_set_nonblocking(SOCKET sock);
int udp_set_blocking(SOCKET sock);

int udp_bind(SOCKET sock, const struct sockaddr_in *addr);
int udp_bind_quick(SOCKET sock, const char *ip, unsigned short port);

int udp_send(SOCKET sock, const struct sockaddr_in *to, const char *buf, size_t bufsize);
int udp_send_temp_sock(const struct sockaddr_in *to, const char *buf, size_t bufsize);
int udp_send_quick(SOCKET sock, const char *ip, unsigned short port, const char *buf, size_t bufsize);
int udp_send_quick_temp_sock(const char *ip, unsigned short port, const char *buf, size_t bufsize);

int udp_receive(SOCKET sock, const struct sockaddr_in *from, char *out, size_t outsize);
int udp_receive_temp_sock(const struct sockaddr_in *from, char *out, size_t outsize);
int udp_receive_quick(SOCKET sock, const char *ip, unsigned short port, char *out, size_t outsize);
int udp_receive_quick_temp_sock(const char *ip, unsigned short port, char *out, size_t outsize);