#if !defined(BOT_CPP)
#define BOT_CPP


#include "utils.hpp"

class Bot {

public:
    Bot();
    static std::string botExecuter(std::string buff, const Client &clt);
    const std::string & getJoke();

private:
    std::vector<std::string>    jokes;
    int                         numJokes;
};

#endif // BOT_CPP