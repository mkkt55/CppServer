#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class Connector{
public:
    void start();
    void serveLoops();
    void stop();
    void setMaxCount(int mCount);
    ~Connector();
private:
    int maxCount = 100;
    int serverStatus = 0;
    int sockIndex = -1;
    sockaddr_in* addrPtr = nullptr;
};