#if !defined(FILETP_HPP)
#define FILETP_HPP

#include "Client.hpp"

void send_file(std::map<int,Client> &clients, std ::vector<std ::string> &commands, Client &cl);
void get_file(std::map<int,Client> &clients, std ::vector<std ::string> &command, Client &cl);

#endif // FILETP_HPP