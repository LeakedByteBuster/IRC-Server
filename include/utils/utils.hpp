#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <vector> // std::vector
#include <string>
#include <sstream>
#include <iostream>


struct pollfd;
struct sockaddr_in;
class Client;

                            //  prints date, time, host, ip and port in STDOUT
void                        serverWelcomeMessage(const struct sockaddr_in &srvSock, int sfd);
                            //  prints on client side
void                        clientWelcomeMessage(unsigned short cfd);
                            //  print IP and host of connected client on server side
void                        printNewClientInfoOnServerSide(const struct sockaddr_in &cltAddr);
                            //  returns current local time
std::string                 geTime();
                            //  Checks if fd.revents == POLLIN
bool                        isReadable(const struct pollfd &fd);
                            //  checks if fd.revents == POLLERR | POLLHUP
bool                        isError(int revents, int fd, int listenFd);
                            //  checks if (revents == POLLIN) && (fd == server fd)
bool                        isNewConnection(const struct pollfd &fd, int srvfd);
                            //  type=0 : <client> || type=1 : nick!~user@hostname
std::string                 getId(Client &clt, int type);
std::vector<std::string>    splitByLines(std::string buff);
std::vector<std::string>    splitBySpace(std::string str);
int                         parseInput(const char *port, std::string pass);
const char *                getBigMsg();
#endif // UTILS_CPP