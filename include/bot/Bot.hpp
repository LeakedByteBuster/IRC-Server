#if !defined(BOT_CPP)
#define BOT_CPP


// #include "/Users/mbousouf/Desktop/IRC-Server/include/Messages/Messages.hpp"

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
