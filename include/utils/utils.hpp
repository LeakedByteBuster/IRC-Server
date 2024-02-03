#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <vector> // std::vector
#include <map>
#include <string>
#include <sstream>
#include <iostream>

#define BYTES_TO_READ   4096

struct pollfd;
struct sockaddr_in;
class Client;
typedef unsigned int nfds_t;

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

std::vector<std::string>    splitByLines(std::string buff);
std::vector<std::string>    splitBySpace(std::string str);
std::vector<std::string>    splitByValue(std::string s, char c);

int                         parseInput(const char *port, std::string pass);
int                         whichCommand(const std::string &first_argument);

const char *                getBigMsg();
bool                        readIncomingMsg(char ptr[], const int id);
void                        deleteClient(std::map<int, std::string> &map, std::vector<struct pollfd> &fds, 
            std::map<int, Client> &clients, std::map<int, std::vector<std::string> > &gbuff,
            nfds_t &nfds, unsigned long i, int &fdsLeft);
bool						parseRegistrationCommands(std::map<int, Client> &clients, 
	    			std::vector<std::string> &str, Client &client, const std::string &pass);
std::pair<std::string, bool>    parseInput(std::string buff, std::map<int, std::string> &map,
                            const std::vector<struct pollfd>  &fds, unsigned long &i);

#endif // UTILS_CPP