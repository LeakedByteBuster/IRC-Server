#include "Server.hpp"
#include <set>
#include <fstream>
#include "Command.hpp"
#include "Bot.hpp"
#include "utils.hpp"
#include "registrationCommands.hpp"
#include <algorithm>
#include "channel.hpp"

std :: vector<std :: string> HandleIncomingMsg(std :: vector <std :: string> &commands,std :: string msg)
{
    std::stringstream   ss(msg);
    std :: string        token;
    std::string        tmp;
    
    std :: getline(ss,token,'\n');
    std::stringstream   parser(token);
        while (getline(parser,tmp,' ')) {
            commands.push_back(tmp);
        }
    return(commands);
}


int check_is_valid(std::string name,std::string key, std::map<std::string,channel> &channelsInServer,Client clt)
{
    channel& foundChannel = channelsInServer[name];
    if (foundChannel.getKey().compare (key) == 0)
        return 1;
    else 
         Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_BADCHANNELKEY));
    return 0;
       
}
int check_modes (std::string name,std::map<std::string,channel> &channelsInServer,Client clt)
{
     channel& foundChannel = channelsInServer[name];
    if (!foundChannel.getInviteMode())
        return (1);
    else 
         Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_INVITEONLYCHAN));
        return (0);
}
int check_limit (std::string name,std::map<std::string,channel> &channelsInServer,Client clt)
{
    channel& foundChannel = channelsInServer[name];
    std::map<int,Client> map_ids = foundChannel.get_id_clients_in_channel();
    // std::map<int,Client> ::iterator it =std::find(map_ids .begin() ,map_ids .end(),clt.fd);
    std::map<int,Client> ::iterator it =map_ids.find(clt.fd);
    if (it!=map_ids .end())
    {
        Server::sendMsg(clt,": already in channel ");
        return (0);
    }
    if (static_cast<int>(map_ids.size()+1)>foundChannel.getLimit())
    {
        Server::sendMsg(clt,": cant access  channel limit ");
        return (0);
    }
    return (1);
}
std::string get_clients_names_in_channel( std::map<std::string,channel> &channelsInServer,std::string name)
{
    // (void) clt ;
    std::vector <std::string>  clients_names;
    std::map <std::string,channel > :: iterator it = channelsInServer.find (name);
    if(it != channelsInServer.end ())
    {
        std::map <int,Client > :: iterator it_c = it->second.get_id_clients_in_channel().begin();
        for (;it_c != it->second.get_id_clients_in_channel().end(); ++it_c)
        {
        //  std::cout << "fd  >>>>>> " << it_c->second.fd << std::endl;
            std::string v_name = it_c->second.nickname;
            if (it_c->second.isOperator == 1)
                v_name = "@"+ v_name;

            clients_names.push_back(v_name);
        }
        // std::cout << "clients names =  >>> " << clients_names[0] <<std::endl;
    }
    std::string result;
    if (!clients_names.empty())
    {
    for (std::vector<std::string>::const_iterator it = clients_names.begin(); it != clients_names.end(); ++it) {
        // std::cout << "channnels clients --->" << *it <<" ";
        result += (*it+" ");
        }   
    // std::cout << result <<std::endl ;
    }
    return result;
}



std::string host_name()
{
    char host[253];
    if (gethostname(host, sizeof(host)) == -1)
        return "";
    return host;
}

int is_client_in_channel (std::string & name,std::map<std::string,channel> &channelsInServer,std::string cltname)
{
    std::map <std::string,channel > :: iterator it = channelsInServer.find (name);
       std::cout << "name" << name <<std::endl;
    if(it != channelsInServer.end ())
    {
        std::map <int,Client > :: iterator it_c = it->second.get_id_clients_in_channel().begin();
        for (;it_c != it->second.get_id_clients_in_channel().end(); ++it_c)
        {
            if (it_c->second.nickname == cltname)
            {
                std::cout << "clt " << it_c->second.nickname << "target " << cltname  << std::endl ; 
                return (1);
            }
        }
    }
    return (0);
}


void sendMsg_to_channel(std::string name,std::map<std::string,channel> &channelsInServer,std::string str, Client clt)
{
    // std::vector <std::string>  clients_names;
    std::map <std::string,channel > :: iterator it = channelsInServer.find (name);
    if(it != channelsInServer.end ())
    {
        std::map <int, Client> :: iterator it_c = it->second.get_id_clients_in_channel().begin();
        for (;it_c != it->second.get_id_clients_in_channel().end(); ++it_c)
        {
            if (it_c->first != clt.fd)
            {
                // :kadigh!~as@197.230.30.146 PRIVMSG #ch0 : hey there
                std::string msg = getId(clt);

                msg.append(" PRIVMSG " + name + " :" + str);
                // std::cout << "msg : " << msg << std::endl;
                Server::sendMsg (it_c->second, msg);
            }
        }
    }
}

Client& get_client(std::string name,std::map<std::string,channel> &channelsInServer,std::string client_name)
{
    std::map <std::string,channel > :: iterator it = channelsInServer.find (name);
    if(it != channelsInServer.end ())
    {
         std::map <int,Client > :: iterator it_c = it->second.get_id_clients_in_channel().begin();
         if (it_c->second.nickname == client_name)
            return it_c->second;
    }
    static Client null_client;
    return null_client;

}


void list (std::map<std::string,channel> &channelsInServer) 
{
    std::map<std::string,channel> ::iterator it = channelsInServer.begin ();
        std::cout << "channels --> " ; 
    for (;it != channelsInServer.end ();++it)
    {
        std::cout << it->first << " ";
    }
    std::cout << "\n";

}


void invite (std::vector<std::string> & commands,std::map<std::string,channel> &channelsInServer, Client &clt)
{
    if (commands.size()  != 3 )
    {
        Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAMS));
        return ;
    }
    if (!parse_channel_name_token (commands[2])||!check_existed_channel(channelsInServer,commands[2]))
    {
        Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NOSUCHCHANNEL));
        return ;
    }
    if (!is_client_in_channel(commands[2] ,channelsInServer, clt.nickname))
    {
        Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NOTONCHANNEL));
        return ;
    }
    // if mode invite only or user  not operator ERR_CHANOPRIVSNEEDED
    if (is_client_in_channel (commands[2],channelsInServer,commands[1]))
    {
            Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_USERONCHANNEL));
            return ;
    }
    std::cout << "invited" << std::endl;
    channelsInServer[commands[2]].get_id_clients_in_channel().insert(std::make_pair (clt.fd,clt));
    clt.inChannel.push_back(commands [2]);
    Server::sendMsg (clt ,":" + host_name() + " 341 " + clt.nickname + " " + commands[1] + " " + commands[2] + " :Inviting " + commands[1] + " to " + commands[2] + "\r\n");
    // Client invited = get_client(commands[1],channelsInServer,commands[2]);
    // Server::sendMsg( invited,":" + clt.nickname + "!" + clt.username + "@" + host_name () + " INVITE " + commands[1] + " :" + commands[2] + "\r\n");
    // //    Server::sendMsg(clt, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " JOIN " + *nameIt);
    // sendMsg_to_channel(commands[2],channelsInServer, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " JOIN " + commands[2] , clt );
    // Server::sendMsg(clt, ":"+ host_name() + " 353 " + clt.nickname + " = " + commands[2] + " :" + get_clients_in_channel (channelsInServer,commands[2]));
    // Server::sendMsg(clt, ":" + host_name() + " 366 " + clt.nickname + " " + commands[2] + " :End of /NAMES list");
    //add target to the channel 
}

void kick(std::vector<std::string> & commands, std::map<std::string,channel> &channelsInServer, Client &clt) 
{
    if (commands.size() == 3 || commands.size() == 4)
    {   
            // std::cout << "commands--->" << commands[0] << std::endl ;
            // std::cout << "commands--->" << commands[1] << std::endl ;
            // std::cout << "commands--->" << commands[2] << std::endl ;
            // std::cout << "commands--->" << commands[3] << std::endl ;
                std::cout << "clt.isOperator  >>>> " << clt.isOperator << std::endl;
        if (parse_channel_name_token (commands[1]) && check_existed_channel(channelsInServer,commands[1]))
        {
            if (is_client_in_channel(commands[1] ,channelsInServer,clt.nickname) && is_client_in_channel (commands[1],channelsInServer, commands[2]))
            {
                        if (clt.isOperator == 1)
                        {
                            clt.inChannel.erase(std::remove(clt.inChannel.begin (),clt.inChannel.end (),commands[1]),clt.inChannel.end());
                            Server::sendMsg (clt, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " KICK " + commands[1] + " " + commands[2] + " :" + clt.nickname);
                            channelsInServer.erase(commands[1]);
                            sendMsg_to_channel(commands[1],channelsInServer, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " KICK " + commands[1] + " " + commands[2] + " :" + clt.nickname , clt );
                            // Server::sendMsg (, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " KICK " + commands[1] + " " + commands[2] + " :" + clt.nickname);
                           
                            // std::cout << "deleted clt n : " << clt.fd << std::endl;
                        }
                        else
                       Server::sendMsg(clt ,Message::getError (clt.nickname, Message ::ERR_CHANOPRIVSNEEDED));
            }
            else
                Server::sendMsg(clt ,Message::getError (clt.nickname, Message ::ERR_USERNOTINCHANNEL));
                /// 401 here no such nick /channel 
        }
        else
            Server::sendMsg(clt ,Message::getError (clt.nickname, Message ::ERR_NOSUCHCHANNEL));
    }
    else
        Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAMS));
}


void join(std::vector<std::string> & commands, std::map<std::string,channel> &channelsInServer, Client &clt)
{
    std::map<std::vector<std::string>,std::vector<std::string> > channel_keys;
    channel_keys = parse_join_command(commands,clt.fd);
    if (!channel_keys.empty ())
    {
       std::map<std::vector<std::string>,std::vector<std::string> > ::iterator it = channel_keys.begin();
        if (it!= channel_keys.end())
        {
            for (std::vector<std::string>::const_iterator nameIt = it->first.begin(), keyIt = it->second.begin(); \
                nameIt != it->first.end() || keyIt != it->second.end(); ++nameIt)
            {
                    std::string lol = keyIt != it->second.end() ? *keyIt : "";
                if (channelsInServer.find (*nameIt) !=  channelsInServer.end()) //channel already exists
                {
                    if (check_is_valid(*nameIt,*keyIt,channelsInServer,clt)&& check_limit(*nameIt,channelsInServer,clt) && check_modes(*nameIt,channelsInServer,clt))
                    {
                            channelsInServer[*nameIt].get_id_clients_in_channel().insert(std::make_pair (clt.fd,clt));
                            clt.inChannel.push_back(*nameIt);
                            Server::sendMsg(clt, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " JOIN " + *nameIt);
                            sendMsg_to_channel(*nameIt,channelsInServer, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " JOIN " + *nameIt , clt );
                            Server::sendMsg(clt, ":"+ host_name() + " 353 " + clt.nickname + " = " + *nameIt + " :" + get_clients_names_in_channel (channelsInServer,*nameIt));
			                Server::sendMsg(clt, ":" + host_name() + " 366 " + clt.nickname + " " + *nameIt + " :End of /NAMES list");
                    }
                }
                else //new channel
                {
                    std::string lol = keyIt != it->second.end() ? *keyIt : "";
                    channel a(clt, *nameIt, *keyIt );
                    channelsInServer[*nameIt] = a;
                    clt.inChannel .push_back (*nameIt);
                    if (!get_clients_names_in_channel (channelsInServer,*nameIt).empty())
                    {
                        Server::sendMsg(clt, ":" + clt.nickname + "!" + clt.username + "@" + host_name() + " JOIN " + *nameIt);
                        Server::sendMsg(clt, ":"+ host_name() + " 353 " + clt.nickname + " = " + *nameIt + " :" + get_clients_names_in_channel (channelsInServer,*nameIt));
			            Server::sendMsg(clt, ":" + host_name() + " 366 " + clt.nickname + " " + *nameIt + " :End of /NAMES list");
                        // sendMsg_to_channel(*nameIt,channelsInServer, ":" + clt.nickname + "!" + clt.username + "@" + "host_name" + " JOIN " + *nameIt , clt );
                    }
                }
                if (keyIt != it->second.end())
                    keyIt++;
            }
        }
    }
}

// int check_is_operator(std::string & commands,Client clt)
// {
//     if (!clt.isOperator)
//     {
//         Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAMS));
//         return 0 ;
//     }
//     return (1);
// }
// void kick(std::vector<std::string> & commands, std::map<std::string,channel> &channelsInServer, Client clt)
// {
//     if (commands.size() < 3)
//          Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAMS));
//     else 
//     {
//         if (parse_channel_name_token(commands[1]) && check_is_operator(commands[2],clt))
//     }
// }
// check command if it's valide and execute it


void execute_commmand(std::map<int,Client> &clients, std ::vector<std ::string> &commands, int id,std::map<int,channel> &channels,std::map<std::string, channel> &channelsInServer)
{
    int res = 0;
    (void)channels;
    if (!commands.empty())
    {

        std ::string first_argument = commands[0];
        std::map<int, Client>::iterator it = clients.find(id);

        if(it == clients.end())
        {
            return;
        }
        
        res =     (first_argument.compare("SENDFILE") == 0) * 1 \
                + (first_argument.compare("GETFILE") == 0)  * 2 \
                + (first_argument.compare("NICK") == 0)     * 3 \
                + (first_argument.compare("PASS") == 0)     * 4 \
                + (first_argument.compare("USER") == 0)     * 4 \
                + (first_argument.compare("PRIVMSG") == 0)   * 5 \
                + (first_argument.compare("join") == 0)   * 6   \
                + (first_argument.compare("JOIN") == 0)   * 6   \
                + (first_argument.compare("kick") == 0)   * 7   \
                + (first_argument.compare("KICK") == 0)   * 7   \
                + (first_argument.compare("list") == 0)   * 8   \
                + (first_argument.compare("INVITE") == 0)   * 9 \
                + (first_argument.compare("invite") == 0)   * 9 \
                + (first_argument.compare("MODE") == 0)   * 11  \
                + (first_argument.compare("PONG") == 0)   * 12 ;

        switch (res)
        {
        case SENDFILE:
            send_file(clients,commands,it->second);
            break;

        case GETFILE:
            get_file(clients,commands,it->second);
            break;

        case NICK:
            try {
                std::string buff;
                for (unsigned long i = 0; i < commands.size(); i++)
                {
                    if (i != 0)
                        buff = buff.append(" ");
                    buff = buff.append(commands[i]);
                }
                parseNick(clients, it->second, buff);
            }
            catch (std::exception &e) { }
            break;
        
        case PASS_USER:
            Server::sendMsg(it->second, Message::getError(it->second.nickname, Message::ERR_ALREADYREGISTRED));
            break;

        case PRIVMSG:
            prv_msg(channelsInServer, commands, it->second,clients);
            break ;

        // case PONG: // ignore PONG
        //     break;
        
        case JOIN:
            join(commands,channelsInServer,it->second);
            break;
        
        case KICK:
            kick(commands,channelsInServer,it->second);
            break;

        case LIST:
            list (channelsInServer);
            break;

        case INVITE: 
            invite (commands,channelsInServer,it->second);
            break;
        
        case PONG: // ignore PONG
            break;
        
        case TOPIC:
            break;
        
        case MODE:
            break;

        default:
            Server::sendMsg(it->second, Message::getError(it->second.nickname, Message::ERR_UNKNOWNCOMMAND));
            break;
        }
    }
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



// SYNTAXE SENDFILE FILENAME  RECIEVER
void send_file(std::map<int,Client> &clients, std ::vector<std ::string> &commands, Client &cl)
{
    std ::FILE *FileName;

    if (commands.size() < 3)
    {
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NEEDMOREPARAM));
        return;
    }
    // open file both binary and text
    FileName = fopen(commands[1].c_str(), "rb");
    if (!FileName)
    {
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NOSUCHFILE));
        return;
    }
    // if not found reciever
    int fd = search_a_client(clients, commands[2]);
    if (!fd)
    {
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NOSUCHNICK));
        return;
    }
    // creat object file and push it in client vector of files
    // fl(FILE *,FILNAME,SENDER,RECEIVER);
    TFile fl(FileName, commands[1].c_str(), cl.nickname, commands[2].c_str());
    std::map<int, Client>::iterator rec = clients.find(fd);
    rec->second.Files.push_back(fl);
    cl.sendMsg(cl, getDownMsg());
}

// SYNTAXE : GETFILE FILENAME SENDER
void get_file(std::map<int,Client> &clients, std ::vector<std ::string> &command, Client &cl)
{
    if (command.size() != 3)
    {
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NEEDMOREPARAM));
        return;
    }
    else if (command[1].empty())
    {
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NOSUCHFILENAME));
        return;
    }
    // if c'ant find the sender of file
    else if (!search_a_client(clients, command[2]))
    {
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NOSUCHNICK));
        return;
    }
    //if there is no files in client vector files
    else if (cl.Files.empty())
    {
        // std::cout << "reciever :"<<cl.fd<<std::endl;
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NOSUCHFILENAME));
        return;
    }
    // if there is no file from sender
    else if (!search_a_file(cl, command[2].c_str()))
    {
        Server::sendMsg(cl, Message::getError(cl.nickname, Message::ERR_NOFILEFROMSENDER));
        return;
    }
    else
    {
        // creat file in reciever /dir
        creat_file(cl, command[2], command[1]);
    }
}

int search_a_file(Client clt, std ::string sender)
{
    std ::vector<TFile>::iterator it = clt.Files.begin();

    for (; it != clt.Files.end(); it++)
    {
        if (it->getSender().compare(sender) == 0)
        {
            return (1);
        }
    }
    return (0);
}

void creat_file(Client clt, std ::string sender, std ::string filename)
{
    int file_size;
    char *line = NULL;
    FILE *fd;
    std ::vector<TFile>::iterator it = clt.Files.begin();
    std::fstream myfile;

    for (; it != clt.Files.end(); it++)
    {
        if (it->getSender().compare(sender) == 0)
        {
            fd = it->getstream();
        }
    }
    // determine size of file
    int prev = ftell(fd);
    std::fseek(fd, 0, SEEK_END);
    file_size = ftell(fd);
    std::fseek(fd, prev, SEEK_SET);
        line = new(std::nothrow)char[file_size];
    if(!line)
    {
        clt.sendMsg(clt, "I think the file is too big can you be kind with us :)");
    }
    // open the new file in client /dir
    myfile.open("transferd_" + filename, std::ios::out | std::ios::binary);
    if (myfile.is_open() == 0)
    {
        clt.sendMsg(clt, "C 'ant open file ");
        myfile.close();
        clt.Files.clear();
        clt.Files.clear();
    }
    // read from sender file
    int readbytes = fread(line, 1, file_size, fd);
    if (readbytes == -1)
    {
        clt.sendMsg(clt, "C'ant read from file");
        myfile.close();
        clt.Files.clear();
    }
    myfile.write(line, file_size);
    myfile.close();
    clt.Files.clear();
}



// search two points in last argument 
int check_text_msg(std ::string msg)
{
    int two_point = std::count(msg.begin(),msg.end(),':');
    return(two_point);
}


void prv_msg(std::map<std::string,channel> &channels, std::vector<std ::string> command, Client clt,std::map<int,Client> clients)
{
    if (command.size() < 3)
    {
        Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NEEDMOREPARAM));
        return;
    }
    if(check_text_msg(command[2]) < 1 && command.size() > 3)
    {
        Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NOTEXTTOSEND));
        return;
    }
    std::vector<std::string> client_and_channels = parse_such(command[1]);
    check_targets(channels,command,client_and_channels,clt,clients);
}


// append all msg argument on command in single string
std :: string compile_msg(std::vector<std::string> commands,int position)
{
    std :: string first = commands[position];
    size_t i = position;

    if(first.front() == ':' && first.at(1) == ':')
    {
        first = first.substr(1,first.size());
        commands[position] = first;
    }
    std :: string msg;
    for(; i < commands.size();i++)
    {
        std::cout << commands[i] << " ";
        if(i != commands.size() - 1)
            msg = msg.append(commands[i] + " ");
        else
            msg = msg.append(commands[i]);
    }
    return(msg);
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

void check_targets(std::map<std::string,channel> channelsinserver, std::vector<std::string> command,std::vector<std::string> params, Client clt,std::map<int,Client> clients)
{
    std :: string msg = compile_msg(command,2);
    for(size_t i = 0; i < params.size();i++)
    {
        // case of channel
        if(params[i].find('#') == 0)
        {
            // check channel if it's on server 
            int id = check_channel(channelsinserver,params[i],clt);
            //channel not found 
            if(!id)
            {
                Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NOSUCHNICK));
                return;
            }
            else if(id == 2) // channel found and the sender is a member on it
            {
                std::map<std::string,channel>::iterator it = channelsinserver.find(params[i]);
                sendMsg_to_channel(it->first,channelsinserver,msg,clt);
                return;
            }
            else // the channel found but the sender is not member on it
            {
                Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_CANNOTSENDTOCHAN));
                return;
            }
        }
        else // case is an other client
        {
            int id = search_a_client(clients,params[i]);
            if(id)
            {
                std::map<int,Client>::iterator it = clients.find(id);
                sendPrvmsg(clt,msg,it->second);
                // send Massege to client;
            }
            else
            {
                Server::sendMsg(clt, Message::getError(clt.nickname, Message::ERR_NOSUCHNICK));
            }
        }
    }
}

const char *getDownMsg()
{

    const char *Down =
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
        " █████ ██╗██╗  █████╗█████╗█████╗███╗  ██╗████████╗\n"
        " ██╔═══██║██║  ██═══╝██═══╝██═══╝████  ██║╚══██╔══╝\n"
        " ████╗ ██║██║  ███╗  █████╗███╗  ██╔█╗ ██║   ██║   \n"
        " ██╔═╝ ██║██║  ██═╝  ╚══██║██═╝  ██║╚█╗██║   ██║   \n"
        " ██║   ██║████ █████╗█████║█████╗██║ ████║   ██║   \n"
        " ╚═╝   ╚═╝╚════╚════╝╚════╝╚════╝╚═╝ ╚═══╝   ╚═╝   \n"
        "⣿⣿⣿⣿⠿⠛⠋⠁⠀⣿⣀⣬⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
        "⣿⣿⣿⡀⠀⠀⠀⠀⠀⠈⣛⣁⣀⣠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
        "⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⣿⠉⠉⠉⠀⠀⠀⢹⡏⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
        "⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⢻⡆⠀⠀⠀⠀⠀⠘⠿⠛⠛⢻⣿⣿⣿⣿⣿⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⢰⡶⠶⠦⠤⢼⣿⣿⣿⣿⣿⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠈⡇⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⠀⢠⡿⢻⣿⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⣿⠀⠀⠀⠀⣾⠇⠀⠀⠀⠀⠀⠸⣧⣤⣽⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⣿⡀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣿⣿⡇⠀⠀⢰⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣾⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣤⣤⣀⣀⣀⣀⠀⠀⠀⠀⣿⣿⣿⣿\n"
        "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n";
    return (Down);
}


//":" + senderNick + "!~" + senderUsername + "@" + senderHostname + " PRIVMSG " + receiver + " :" + message + "\r\n"
void sendPrvmsg(Client sender,std::string str,Client recv)
{ 
    // append user prefix for limechat 
    std::string msg = getId(sender);
    msg.append(" PRIVMSG " + recv.nickname + " " + str);

    Server::sendMsg(recv, msg);

    // send rply to client 
    Server::sendMsg(sender, Message::rplAwayMsg(sender, msg));
}

int check_channel(std::map<std::string,channel> channles_server,std::string channel_name,Client clt)
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