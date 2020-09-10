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
    ~Connector();
private:
    int serverSocket = -1;
};