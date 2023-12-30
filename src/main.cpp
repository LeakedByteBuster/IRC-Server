#include "InputOutput.hpp"
#include "Server.hpp"

/*
    parseInput();
    initServer();
    checkClientInput(); // PASS, USERNAME....
    checkIfValidCommand();
    ...

*/

int main(int ac, char **av)
{
    try {
        if (ac != 3)
            throw std::invalid_argument("Error : ./irc <port> <password>");

        Server  srv(av[1], av[2]);

    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << " " << std::endl;
        return (1);
    } catch (std::exception &e) {
        std::cerr << e.what () << std::endl;
        return (2);
    }

    return (0);
}
