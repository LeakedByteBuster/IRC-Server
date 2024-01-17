#include <iostream>
#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <arpa/inet.h> // struct sockaddr_in

static int  parseInput(const char *s, std::string pass)
{
    char    *endptr = NULL;
    long res = strtol(s, &endptr, 10);
    if ((endptr && *endptr != '\0') || (res > USHRT_MAX)
        || (res < 0)) {
        throw std::invalid_argument("Error : invalid port number");
    }

    if (pass.length() == 0) {
        throw std::invalid_argument("Error : empty password");
    }
    return (0);
}

int main(int ac, char **av) 
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);

    if (ac != 4) {
        std::cerr << "Error :invalid number of arguments :<serverip> <port> <pass>"
            << std::endl;
        return (1);
    }
    memset(&addr, 0, sizeof(addr));
    try {
        parseInput(av[2], av[3]);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return (1);
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::cerr << "Error socket():" << strerror(errno) << std::endl;
        return (2);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(av[2]));
    if (inet_pton(addr.sin_family, av[1], &addr.sin_addr) < 0) {
        std::cerr << "Error inet_pton(): " << strerror(errno) << std::endl;
        return (2);
    }

    int status = 0;
    if ((status = connect(fd, (const sockaddr *)&addr, len)) == -1) {
        std::cerr << "Error connect(): " << strerror(errno) << std::endl;
        return (2);
    }

    std::cout << "Connected to the server successfully" << std::endl;
    // while (1) {
    //     sleep(60);

    // }

    return (0);
}