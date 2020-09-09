#include "Connector.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

void Connector::setMaxCount(int mCount) {
    maxCount = mCount;
}

void Connector::start() {
    std::cout << "This is server" << std::endl;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Error: socket" << std::endl;
        return;
    }
    addrPtr = new sockaddr_in();
    // Make sure you are passing the same struct to bind()
    // when using variable "addr" and addrPtr at the same time.
    // sockaddr_in addr = *addrPtr;
    addrPtr->sin_family = AF_INET;
    addrPtr->sin_port = htons(8000);
    addrPtr->sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)addrPtr, sizeof(*addrPtr)) == -1) {
        std::cout << "Error: bind" << std::endl;
        stop();
        return;
    }

    if(listen(serverSocket, 5) == -1) {
        std::cout << "Error: listen" << std::endl;
        stop();
        return;
    }

    serverStatus = 1;
    std::cout << "Server on listening" << std::endl;

    serveLoops();
}

void Connector::serveLoops() {
    int conn;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while (true) {
        conn = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (conn < 0) {
            std::cout << "Error: accept" << std::endl;
            continue;
        }
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "Connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

        char buf[255];
        while (true) {
            memset(buf, 0, sizeof(buf));
            int len = recv(conn, buf, sizeof(buf), 0);
            buf[len] = '\0';
            if (strcmp(buf, "exit") == 0) {
                std::cout << "...disconnect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
                break;
            }
            std::cout << buf << std::endl;
            send(conn, buf, len, 0);
        }
        
        close(conn);
    }
}

void Connector::stop() {
    if (addrPtr != nullptr) {
        delete addrPtr;
        addrPtr = nullptr;
    }

    if (serverSocket != -1) {
        close(serverSocket);
    }

    serverStatus = 0;
}

Connector::~Connector() {
    if (serverStatus != 0) {
        stop();
        serverStatus = 0;
    }
}