#ifndef NETWORK_H
#define NETWORK_H

#include "sys/types.h"
#include <netdb.h>

void initAddrInfo(struct addrinfo **resultList);
int createINETSock();
void resetPort(int sock);
int bindSock(int sock, struct addrinfo *addrStruct);
void createConnection(int sock, struct addrinfo *addrStruct);
int acceptConnection(int sock, struct sockaddr_storage *incomingAddr);
void getRequestData(int fd, char *buffer, size_t size);

#endif
