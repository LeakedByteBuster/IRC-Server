#include "Server.hpp"

void prv_msg(std::vector<std ::string> command , Client clt, std::map<int,Client> clients)
{
    if (command.size() < 3)
    {
        Server::sendMsg(clt, _ERR(clt.nickname, Message::NEEDMOREPARAMS));
        return;
    }
    if(check_text_msg(command[2]) < 1 && command.size() > 3)
    {
        Server::sendMsg(clt, _ERR(clt.nickname, Message::ERR_NOTEXTTOSEND));
        return;
    }
    std::vector<std::string> client_and_channels = parse_such(command[1]);
    check_targets(command,client_and_channels,clt,clients);
}



void check_targets(std::vector<std::string> command , std::vector<std::string> params , Client clt , std::map<int,Client> clients)
{

    bool Operator = false;
    std :: string Chnl_name;

    std :: string msg = compile_msg(command,2);
    for(size_t i = 0; i < params.size();i++)
    {
        // case of channel
        if(params[i].find('#') == 0 || (params[i].find('@') == 0 && params[i].find('#') == 1))
        {
            //check if name of channle come with operator prefix
            Operator = params[i].find("@") == 0 ? true : false;
            Chnl_name = Operator ? params[i].substr(1,params[i].size()) : params[i];
            // check channel if it's on server
            int id = check_channel(Server::ChannelsInServer , Chnl_name,clt);

            if(!id) // Channel Not found
            {
                Server::sendMsg(clt, _ERR(clt.nickname , ERR_NOSUCHNICK));
                return;
            }
            else if(id == 2) // channel found and the sender is a member on it
            {
                std::map<std::string,Channel>::iterator it = Server::ChannelsInServer.find(Chnl_name);

                if(it != Server::ChannelsInServer.end()){

                    msg = commandReply(it->second , clt , msg , TYPE_USER);
                    Detrm_Dest_Msg(it->second , msg , Operator);
                }
                else{

                    Server::sendMsg(clt, _ERR(clt.nickname , ERR_NOSUCHNICK));
                }
                return;
            }
            else // the channel found but the sender is not member on it
            {
                Server::sendMsg(clt, _ERR(clt.nickname, ERR_CANNOTSENDTOCHAN));
                return;
            }
        }
        else // case is an other client
        {
            int id = search_a_client(clients,params[i]);
            if(id)
            {
                std :: cout << "Client :"<<id<<std::endl;
                
                std::map<int,Client>::iterator it = clients.find(id);
                sendPrvmsg(clt , msg , it->second);
                // send Massege to client;
            }
            else
            {
                Server::sendMsg(clt , _ERR(clt.nickname, Message::ERR_NOSUCHNICK));
            }
        }
    }
}


// Determine the destination of the message is just Operator or all client
void Detrm_Dest_Msg(Channel ChnlDest , std::string Msg,bool Operator)
{
    std::map<int,Client>::iterator it = ChnlDest.clientsInChannel.begin();
    if(Operator)
    {
        for(; it != ChnlDest.clientsInChannel.end(); it++)
        {
            if(it->second.isOperator)
            {
                Server::sendMsg(it->second,Msg);
            }
        }
    }
    else
    {
      Server::sendMsg(ChnlDest,Msg);
    }
}




// <':'><nick!~user><@><hostname> <command> <nickname>
std::string privmsgReply(const Client &sender , const Client &recv ,  const std::string &msg)
{
    std::string rpl(":"); 
    rpl.append( userPrefix(sender) + " " + "PRIVMSG" + " " + recv.nickname +" "+ msg);

    return (rpl);
}


//":" + senderNick + "!~" + senderUsername + "@" + senderHostname + " PRIVMSG " + receiver + " :" + message + "\r\n"
void sendPrvmsg(Client sender , std::string str , Client recv)
{
    //add privmsg command reply to messages
    std::string Final_Msg = privmsgReply(sender , recv , str);
    Server::sendMsg(recv , Final_Msg);

    // send rply to client
    Server::sendMsg(sender, Message::rplAwayMsg(sender, recv.nickname));
}







