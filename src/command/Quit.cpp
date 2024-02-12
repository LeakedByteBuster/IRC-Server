#include "Server.hpp"


//Closing Link: 197.230.30.146 (Client Quit)
// one has left IRC (Client Quit)

//  :dan-!d@localhost QUIT :Quit: Bye for now!
// :one!~teo@197.230.30.146 QUIT :Client Quit

void DeleteClt(Channel &Chnl,Client clt)
{
    std::map<int,Client>::iterator it = Chnl.clientsInChannel.find(clt.fd);
    if(it != Chnl.clientsInChannel.end() )
    {
        Chnl.clientsInChannel.erase(it);
    }
}

std :: string Quitprefix(Client &clt)
{
    std::string prefix;
    prefix.append(":");
    prefix.append(userPrefix(clt));
    prefix.append(" QUIT ");
    return(prefix);
}

void Client_Quit(Client & clt,std::vector<std::string> command,std::map<int,Client> clients)
{
    std :: string reason;
    std :: string chnlmsg;

    reason.append(Quitprefix(clt));


    if(command.size() > 1 && command[1].size() > 1)
        reason.append(compile_msg(command,1));
    else
        reason.append("Client Quit");
    Server::sendMsg(clt,reason);
    std::map<std::string,Channel>::iterator it = clt.ChannelIn.begin();
    for(; it != clt.ChannelIn.end();it++)
    {
        Server::sendMsg(it->second,clt,reason);
        DeleteClt(it->second,clt);
    }
    clients.erase(clt.fd);
}