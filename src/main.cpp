#include <climits>
#include "Server.hpp"

static int  parseInput(const char *s, std::string pass)
{
    char    *endptr = NULL;
    long res = strtol(s, &endptr, 10);
    if ((endptr && *endptr != '\0') || (res > USHRT_MAX)
        || (res < 0)) {
        std::cout << s << " |  " << endptr << std::endl;
        throw std::invalid_argument("Error : invalid port number");
    }

    if (pass.length() == 0) {
        throw std::invalid_argument("Error : empty password");
    }
    return (0);
}

int main(int ac, char **av)
{
    try {
        if (ac != 3)
            throw std::invalid_argument("Error : ./irc <port> <password>");

        parseInput(av[1], av[2]);
        Server  srv(htons(atoi(av[1])), static_cast<std::string>(av[2]));
        srv.handleIncomingConnections();

    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << " " << std::endl;
        return (1);
    } 
    catch (std::runtime_error &e) {
        std::cerr << e.what() << " " << std::endl;
        return (2);
    } catch (std::exception &e) {
        std::cerr << e.what () << std::endl;
        return (2);
    } catch (...) {
        std::cerr << "Exception occurred" << std::endl;
        return (2);
    }

    return (0);
}
