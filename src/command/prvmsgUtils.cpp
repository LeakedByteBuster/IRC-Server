#include "Server.hpp"

// int search_in_channels(std::map<int,channel> channels ,std::string name,Client clt)
// {
//     if(!channels.empty())
//     {
//         std::map<int,channel>::iterator it= channels.begin();

//         for(;it != channels.end();it++)
//         {
//             if(!it->second.getName().compare(name))
//             {
//                 int id = search_client_inChannel(clt,it->second);
//                 if(id)
//                 {
//                     return id;
//                 }
//                 else
//                 {
//                     Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_CANNOTSENDTOCHAN));
//                     return 0;
//                 }
//             }
//         }
//         if(it == channels.end())
//         {
//             Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NOSUCHNICK));
//                 return 0;
//         }
//     }
//     return(0);
// }

// int search_client_inChannel(Client clt,channel channel)
// {
//     std :: vector<int> vec = channel.get_id_clients_in_channel();
//     std :: vector<int>::iterator it = vec.begin();

//     for(; it != vec.end();it++)
//     {
//         if(*it == clt.fd)
//         {
//             return clt.fd;
//         }
//     }
//     return(0);
// }