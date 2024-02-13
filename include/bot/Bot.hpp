#if !defined(BOT_CPP)
#define BOT_CPP

#include <vector>
#include <sstream>



#define BYTES_TO_READ   4096
/* Sockets */
#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <sys/types.h> // bind()
#include <arpa/inet.h> // inet_aton(), htons()
#include <netdb.h> // getaddinfo()
#include <unistd.h>


#include <iostream>

class Server;

class Bot {

public:
    Bot();
    std::string botExecuter(std::string buff, std::string clientName);
    const std::string & getJoke();

private:
    std::vector<std::string>    jokes;
    int                         numJokes;
};

#endif // BOT_CPP
#define ERR_INVALIDARGUMENTS "Error :invalid number of arguments :<serverip> <port> <pass>"
