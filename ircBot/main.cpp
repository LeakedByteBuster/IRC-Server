#include <iostream>
#include <sys/socket.h> // socket(), bind(), listen(), connect(), accept()
#include <arpa/inet.h> // struct sockaddr_in
#include <unistd.h>

#define ERR_INVALIDARGUMENTS "Error :invalid number of arguments :<serverip> <port> <pass>"

enum    Command {
    JOKES,
    TIME,
    WELCOME
};

static bool    sendRegistrationInfo(const int fd, char *pass)
{
    std::string regs;

    regs = "PASS " + static_cast<std::string>(pass)
            + static_cast<std::string>("\r\n");
    regs.append("NICK bot\r\n");
    regs.append("USER bot 0 * bot\r\n");

    std::cout << "Connected to the server successfully" << std::endl;

    if (send(fd, regs.data(), regs.size(), 0) == -1) {
        std::cerr << "Error send(): " << strerror(errno) << std::endl;
        return (0);
    }
    return (1);
}

static int  parseInput(const char *port, std::string pass)
{
    char    *endptr = NULL;
    long res = strtol(port, &endptr, 10);
    if ((endptr && *endptr != '\0') || (res > USHRT_MAX)
        || (res < 0)) {
        throw std::invalid_argument("Error : invalid port number");
    }

    if (pass.length() == 0) {
        throw std::invalid_argument("Error : empty password");
    }
    return (0);
}

static int connectToServer(const char *ip, const char *port,
        const char *pass, struct sockaddr_in &addr, socklen_t len)
{
    memset(&addr, 0, sizeof(addr));
    try {
        parseInput(port, pass);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return (0);
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::cerr << "Error socket():" << strerror(errno) << std::endl;
        return (0);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    if (inet_pton(addr.sin_family, ip, &addr.sin_addr) < 0) {
        std::cerr << "Error inet_pton(): " << strerror(errno) << std::endl;
        return (0);
    }

    if (connect(fd, (const sockaddr *)&addr, len) == -1) {
        std::cerr << "Error connect(): " << strerror(errno) << std::endl;
        return (0);
    }
    return (fd);
}

int main(int ac, char **av) 
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);
    int                 fd = 0;
    char                buff[4096];
    int                 bytes = 0;

    if (ac != 4) {
        std::cerr << ERR_INVALIDARGUMENTS << std::endl;
        return (1);
    }
    if ((fd = connectToServer(av[1], av[2], av[3], addr, len)) == 0) {
        return (2);
    }
    sleep(6);
    if (!sendRegistrationInfo(fd, av[3])) {
        return (2);
    }

    while (1) {
        memset(buff, 0, sizeof(buff));
        std::cout << "Waiting..." << std::endl;
        if ((bytes = recv(fd, static_cast<void *>(buff), sizeof(buff) - 1, 0)) == -1) {
            std::cerr << "Error recv(): " << strerror(errno) << std::endl;
            return (2);
        } else if (bytes == 0) {
            std::cout << "Server disconnected" << std::endl;
            break ;
        }
        std::cout << buff << std::endl;
        std::cout.flush();
        // enumtype  parseBuff();
        // void      executeBuff();
    }

    return (0);
}