#include "udpx_udp.h"

int CreateServerAddressStruct(const char *ip, unsigned short port, struct sockaddr_in *server_addr) {
	memset(server_addr, 0, sizeof(*server_addr));
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(port);
	int result = InetPtonA(AF_INET, ip, &server_addr->sin_addr.s_addr);
	if (result != 1) return -1;
	return result;
}

SOCKET udp_create_socket() {
	return socket(AF_INET, SOCK_DGRAM, 0);
}
void udp_close_socket(SOCKET sock) {
	if (sock < 0) return;
	closesocket(sock);
}
int udp_shutdown_socket(SOCKET sock) {
	if (sock < 0) return -1;
	if (shutdown(sock, SD_BOTH) < 0) return -1;
	return 0;
}

int udp_set_nonblocking(SOCKET sock) {
	u_long mode = 1;
	if (ioctlsocket(sock, FIONBIO, &mode) != 0) return -1;
	return 0;
}
int udp_set_blocking(SOCKET sock) {
	u_long mode = 0;
	if (ioctlsocket(sock, FIONBIO, &mode) != 0) return -1;
	return 0;
}

int udp_bind(SOCKET sock, const struct sockaddr_in *addr) {
	return bind(sock, (struct sockaddr *)addr, sizeof(*addr));
}
int udp_bind_quick(SOCKET sock, const char *ip, unsigned short port) {
	struct sockaddr_in server_addr;
	if (CreateServerAddressStruct(ip, port, &server_addr) < 0) return -1;
	return udp_bind(sock, &server_addr);
}

int udp_send(SOCKET sock, const struct sockaddr_in *to, const char *buf, size_t bufsize) {
	return sendto(sock, buf, bufsize, NULL, (struct sockaddr *)to, sizeof(*to));
}
int udp_send_temp_sock(const struct sockaddr_in *to, const char *buf, size_t bufsize) {
	SOCKET sock = udp_create_socket();
	if (sock < 0) return -1;
	int ret = udp_send(sock, to, buf, bufsize);
	udp_shutdown_socket(sock);
	udp_close_socket(sock);
	return ret;
}
int udp_send_quick(SOCKET sock, const char *ip, unsigned short port, const char *buf, size_t bufsize) {
	struct sockaddr_in server_addr;
	if (CreateServerAddressStruct(ip, port, &server_addr) < 0) return -1;
	return udp_send(sock, &server_addr, buf, bufsize);
}
int udp_send_quick_temp_sock(const char *ip, unsigned short port, const char *buf, size_t bufsize) {
	struct sockaddr_in server_addr;
	if (CreateServerAddressStruct(ip, port, &server_addr) < 0) return -1;
	return udp_send_temp_sock(&server_addr, buf, bufsize);
}

int udp_receive(SOCKET sock, const struct sockaddr_in *from, char *out, size_t outsize) {
	int fromlen = sizeof(*from);
	return recvfrom(sock, out, outsize, MSG_WAITALL, (struct sockaddr *)from, &fromlen);
}
int udp_receive_temp_sock(const struct sockaddr_in *from, char *out, size_t outsize) {
	SOCKET sock = udp_create_socket();
	if (sock < 0) return -1;
	int ret = udp_receive(sock, from, out, outsize);
	udp_shutdown_socket(sock);
	udp_close_socket(sock);
	return ret;
}
int udp_receive_quick(SOCKET sock, const char *ip, unsigned short port, char *out, size_t outsize) {
	struct sockaddr_in server_addr;
	if (CreateServerAddressStruct(ip, port, &server_addr) < 0) return -1;
	return udp_receive(sock, &server_addr, out, outsize);
}
int udp_receive_quick_temp_sock(const char *ip, unsigned short port, char *out, size_t outsize) {
	struct sockaddr_in server_addr;
	if (CreateServerAddressStruct(ip, port, &server_addr) < 0) return -1;
	return udp_receive_temp_sock(&server_addr, out, outsize);
}
