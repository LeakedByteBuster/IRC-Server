#if !defined(PART_HPP)
#define PART_HPP

#include "Server.hpp"

std :: string Partprefix(Client &clt);
void Part_client(Client & clt, std::vector<std::string> command);
#endif