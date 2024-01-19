#include "Server.hpp"

int main(int ac, char **av)
{
    try
    {
        if (ac != 3)
            throw std::invalid_argument("Error : ./irc <port> <password>");

        parseInput(av[1], av[2]);
        Server srv(av[1], static_cast<std::string>(av[2]));
        srv.handleIncomingConnections();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (2);
    }
    catch (...)
    {
        std::cerr << "Exception occurred" << std::endl;
        return (2);
    }

    return (0);
}
