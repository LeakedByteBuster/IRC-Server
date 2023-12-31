#include <cstring>
#include "Server.hpp"

void    initSockAddrStruct(struct sockaddr_in *sock, unsigned short lport) {
    memset(sock, 0, sizeof(*sock));
    sock->sin_family = AF_INET;
    sock->sin_port = lport;
    sock->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}