#include "Server.hpp"
#include <algorithm>


// append all msg argument on command in single string
std :: string compile_msg(std::vector<std::string> commands, int position)
{
    std :: string msg;

    for(size_t i = position; i < commands.size();i++)
    {   
        if(i != commands.size() - 1) {
            msg.append(commands[i] + " ");
            continue ;
        }
        msg.append(commands[i]);
    }
    if(std::count(msg.begin(), msg.end(), ':') > 0) {
        std::string::iterator it = std::find(msg.begin(), msg.end(), ':');
        msg.erase(it);
    }
    return (msg);
}




int check_channel(std::map<std::string,Channel> channles_server,std::string channel_name,const Client &clt)
{
    // check the channel if it's on server
    if (check_existed_channel(channles_server,channel_name))
    {
        // check the sender (client) is a membre on channe;
       if(is_client_in_channel(channel_name,channles_server,clt.nickname))
       {
            return(2);
       }
       else
       {
            return(1);
       }
    }
    return 0;
}



// search two points in last argument
int check_text_msg(std ::string msg)
{
    int two_point = std::count(msg.begin(),msg.end(),':');
    return(two_point);
}

// parse the second argument of command (split channel and client)
std::vector<std::string> parse_such(std::string str)
{
    std::stringstream virgule(str);

    std :: string word;
    std :: vector<std::string> twords;

    while(std::getline(virgule,word,','))
    {
        twords.push_back(word);
    }
    return(twords);
}





int is_client_in_channel (std::string & name,std::map<std::string,Channel> &channelsInServer,std::string cltname)
{
    std::map <std::string,Channel > :: iterator it = channelsInServer.find (name);
    if(it != channelsInServer.end ())
    {
        std::map <int,Client > :: iterator it_c = it->second.clientsInChannel.begin();
        for (;it_c != it->second.clientsInChannel.end(); ++it_c)
        {
            if (it_c->second.nickname == cltname)
            {
                return (1);
            }
        }
    }
    return (0);
}

int check_existed_channel (std::map<std::string,Channel> &channelsInServer , std::string name)
{
    std::map<std::string, Channel>::iterator iter = channelsInServer.find(name);
    std::map<int,Client> clt_in;
    if (iter != channelsInServer.end())
    {
        return(1);
    }
    return 0;
}   


// search for a client by his nickname
int search_a_client(std::map<int,Client> clients, std ::string NickName)
{
    if(clients.empty())
    {
        return(0);
    }
        std::map<int, Client>::iterator it = clients.begin();
        for (; it != clients.end(); it++)
        {
            if (it->second.nickname.compare(NickName) == 0)
            {
                return (it->second.fd);
            }
        }
    return (0);
}

// <':'><nick!~user><@><hostname> <nickname> <message>
std::string privmsgReply(const Client &sender , const Client &recv ,  const std::string &msg)
{
    std::string rpl(":"); 
    rpl.append( userPrefix(sender) + " " + "PRIVMSG" + " " + recv.nickname +" :"+ msg);

    return (rpl);
}

// <':'><nick!~user><@><hostname>  <channelname> <message>
std::string ChnlReply(const Client &sender , const Channel &recv ,  const std::string &msg)
{
    std::string rpl(":"); 
    rpl.append( userPrefix(sender) + " " + "PRIVMSG" + " " + recv.name +" :"+ msg);

    return (rpl);
}