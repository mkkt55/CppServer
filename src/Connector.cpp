#include "Connector.h"

void Connector::setMaxCount(int mCount) {
    maxCount = mCount;
}

void Connector::start() {
    std::cout << "This is server" << std::endl;

    sockIndex = socket(AF_INET, SOCK_STREAM, 0);
    if (sockIndex == -1) {
        std::cout << "Error: socket" << std::endl;
        return;
    }
    addrPtr = new sockaddr_in();
    sockaddr_in addr = *addrPtr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockIndex, (sockaddr*)addrPtr, sizeof(addr)) == -1) {
        std::cout << "Error: bind" << std::endl;
        stop();
        return;
    }

    if(listen(sockIndex, 5) == -1) {
        std::cout << "Error: listen" << std::endl;
        stop();
        return;
    }

    serverStatus = 1;
    std::cout << "Server on listening" << std::endl;

    serveLoops();
}

void Connector::serveLoops() {

}

void Connector::stop() {
    if (addrPtr != nullptr) {
        delete addrPtr;
        addrPtr = nullptr;
    }

    if (sockIndex != -1) {
        close(sockIndex);
    }

    serverStatus = 0;
}

Connector::~Connector() {
    if (serverStatus != 0) {
        stop();
        serverStatus = 0;
    }
}