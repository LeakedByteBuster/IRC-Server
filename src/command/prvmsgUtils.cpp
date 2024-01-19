#include "Command.hpp"



int search_in_channels(Server *srv,std::string name,Client clt)
{
    std::map<int,channel>::iterator it= srv->channles.begin();

    for(;it != srv->channles.end();it++)
    {
        if(!it->second.getName().compare(name))
        {
            int id = search_client_inChannel(clt,it->second);
            if(id)
            {
                return id;
            }
            else
            {
                Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_CANNOTSENDTOCHAN));
                return 0;
            }
        }
    }
    if(it == srv->channles.end())
    {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NOSUCHNICK));
            return 0;
    }
    return(0);
}

int search_client_inChannel(Client clt,channel channel)
{
    std :: vector<int> vec = channel.get_id_clients_in_channel();
    std :: vector<int>::iterator it = vec.begin();

    for(; it != vec.end();it++)
    {
        if(*it == clt.fd)
        {
            return clt.fd;
        }
    }
    return(0);
}