#include "Connector.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

void Connector::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Error: socket" << std::endl;
        return;
    }
    sockaddr_in serverAddr;
    // serverAddr will be copied into kernel.
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cout << "Error: bind" << std::endl;
        stop();
        return;
    }

    if(listen(serverSocket, 5) == -1) {
        std::cout << "Error: listen" << std::endl;
        stop();
        return;
    }

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
                std::cout << "Disconnect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
                break;
            }
            std::cout << buf << std::endl;
            send(conn, buf, len, 0);
        }

        close(conn);
    }
}

void Connector::stop() {
    if (serverSocket != -1) {
        close(serverSocket);
        serverSocket = -1;
    }
}

Connector::~Connector() {
    stop();
}