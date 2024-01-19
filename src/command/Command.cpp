#include "Server.hpp"
#include <set>
#include <fstream>
#include "registrationCommands.hpp"
// #include "client.hpp"
// std::vector<int> channel::get_id_clients_in_channel;


//split command into vector of string to check it
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

void sendError(int fd, std::string error,std::string prefix)
{
    std::string msg = prefix + error;
    int i = write (fd,msg.c_str(),msg.size());
    if (i == -1)
        std::cout << "write Error \n";
}

        // if (key.length ()<4)
        //     std::cout << "PASS must be more than 4 characters\n";
    // for (size_t i =1 ; i < key.length () ;i++)
    // {
    //     if (isalnum (key[0]) &&(isalnum(key[i])|| (key[i]== ','  && isalnum(key[i+1])))) 
    //     {
    //             std::cout << "i : " << i << "\n";
    //         // if(key [i] == ',')
    //         // {
    //             std :: cout << "------- key --------" << key [i]<< std :: endl;
    //             validKeys.push_back(key);
    //         // }
    //         // else 
    //         // {
    //         //     std::cout << "param error \n";
    //         //     return std::vector<std::string>();
    //         // }
    //      }
    //     else
    //     {
    //         std::cout << "all keys must be alphanumeric separated by comma\n";
    //         return std::vector<std::string>();
    //     }
    // }
    // return validKeys;


// bool  check_if_exist(std::vector<std::string> &channels)
// {
//     // std::find()

// }

int check_is_valid(std::string name,std::string key, std::map<std::string,channel> &channelsInServer,int id )
{
    channel& foundChannel = channelsInServer[name];
    if (foundChannel.getKey().compare (key) == 0)
        return 1;
    else 
        sendError(id,ERR_BADCHANNELKEY,name);
    return 0;
       
}
int check_modes (std::string name,std::map<std::string,channel> &channelsInServer)
{
     channel& foundChannel = channelsInServer[name];
     if (!foundChannel.getInviteMode() && !foundChannel.getkeyMode())
        return (1);
    else 
        return (0);
}
int check_limit (std::string name,std::map<std::string,channel> &channelsInServer,int id )
{
    channel& foundChannel = channelsInServer[name];
    std::__1::vector<int> vector_ids = foundChannel.get_id_clients_in_channel();
    std::__1::vector<int> ::iterator it =std::find(vector_ids .begin() ,vector_ids .end(),id);
    for (size_t i =0; i <vector_ids.size();i++)
        std::cout << "vec >>> " <<vector_ids[i] << "\n";
    if (it!=vector_ids .end())
    {
        std::cout << "already in channel\n" << name << "\n";
        return (0);
    }
    if (static_cast<int>(vector_ids.size()+1)>foundChannel.getLimit())
    {
        std::cout << "cant access  channel limit  \n";
        return (0);
    }

    // }
    return (1);
}

void parse_command(std::vector<std::string> & commands, std::map<std::string,channel> &channelsInServer, int id)
{
    std::map<std::vector<std::string>,std::vector<std::string> > channel_keys;
    if (!commands.empty())
    {
        if (commands.front() == "JOIN" || commands.front() == "join")
        {
            channel_keys = parse_join_command(commands,id);
            if (!channel_keys.empty ())
            {
            std::map<std::vector<std::string>,std::vector<std::string> > ::iterator it = channel_keys.begin();
                if (it!= channel_keys.end())
                {
                    for (std::vector<std::string>::const_iterator nameIt = it->first.begin(), keyIt = it->second.begin(); \
                        nameIt != it->first.end() || keyIt != it->second.end(); ++nameIt)
                    {
                            std::string lol = keyIt != it->second.end() ? *keyIt : ""; 
                        if (channelsInServer.find (*nameIt)!=  channelsInServer.end()) //channel already exists
                        {
                            if (check_is_valid(*nameIt,*keyIt,channelsInServer,id)&& check_limit(*nameIt,channelsInServer,id) && check_modes(*nameIt,channelsInServer))
                                {
                                   std::vector<int>& channelData = channelsInServer[*nameIt].get_id_clients_in_channel();
                                   channelData.push_back (id);
                                    channelData.insert(channelData.end(), 0, id);
                                   for (unsigned long i = 0; i < channelsInServer[*nameIt].get_id_clients_in_channel().size(); i++) {
                                        std::cout << "id : " << *nameIt << " value ==> " <<  channelsInServer[*nameIt].get_id_clients_in_channel()[i] << std::endl;
                                   }
                                    // std::cout << "client :"  << id <<"joined channel succesfly \n";
                                }
                            else 
                                sendError(id,ERR_BADCHANNELKEY,"111112");
                        }
                        else //new channel
                        {
                            // std::cout <<"not found \n";
                            std::string lol = keyIt != it->second.end() ? *keyIt : ""; 
                            channel a(id,*nameIt,*keyIt ,1);
                            channelsInServer[*nameIt] = a;
                        }
                        if (keyIt != it->second.end())
                            keyIt++;
                    }
                    std::cout << "\n";
                }
            else
            {
                sendError(id,ERR_BADCHANNELKEY,"");
            }
            // else 
            //     channel a(id,,)

            // }
        }
    }
    // else if (firstCommand == "KICK")
    // {
    //     if (commands.size () == 1 )
    //     {
    //         sendError(id ,ERR_NEEDMOREPARAMS,commands[0]);
    //         return; 
    //     }
    //     else if (commands.size () == 2)
    //     {
    //         if (check_channel_name_token (commands[1]))
    //             check_users(commands[2]);

    //     }
    //     else if (commands.size () == 3)
    //     {
    //         if (check_channel_name_token (commands[1]) && check_is_valid)
    //         {
    //             check_users(commands[2]);
    //             add_comment (command[3]);
    //         }

    //     }

    // }
    // else if (firstCommand == "MODE")
    //     check_channel_name (commands);
    // else if (firstCommand == "INVITE")
    //     check_channel_name (commands);
    else 
        sendError(id,"command not found \n","");
}
}

// check command if it's valide and exucte it
void    execute_commmand(Server *sev, std :: vector<std :: string> &commands, int id)
{
    int res = 0;

    if(!commands.empty()) {

        std :: string first_argument = commands[0];
        std::map<int,Client>::iterator it = sev->clients.find(id);
        
        if(it == sev->clients.end()) {
            std :: cout << "No such client" << std::endl;
        }
        
        res =     (first_argument.compare("SENDFILE") == 0) * 1 \
                + (first_argument.compare("GETFILE") == 0)  * 2 \
                + (first_argument.compare("NICK") == 0)     * 3 \
                + (first_argument.compare("PASS") == 0)     * 4 \
                + (first_argument.compare("USER") == 0)     * 4 \
                + (first_argument.compare("PRVMSG") == 0)   * 5 \
                + (first_argument.compare("join") == 0)   * 6 \
                + (first_argument.compare("JOIN") == 0)   * 6 ;

        switch (res)
        {
        case 1:
            send_file(sev,commands,it->second);
            break;
        
        case 2:
            get_file(sev,commands,it->second);
            break;
        
        case 3:
            try {
                std::string buff;
                for (unsigned long i = 0; i < commands.size(); i++) {
                    if (i != 0)
                        buff = buff.append(" ");
                    buff = buff.append(commands[i]);
                }
                parseNick(sev->clients, it->second, buff);
            } catch (std::exception &e) { }
            break;
        
        case 4:
            Server::sendMsg(it->second, LogError::getError(it->second.nickname, LogError::ERR_ALREADYREGISTRED));
            break;
        
        case 5:
            // prv_msg(sev,commands,id);
            break;
        
        case 6:
            parse_command(commands,sev->channelsInServer,id);
            break;
        
        default:
            Server::sendMsg(it->second,": COMMAND NOT FOUND !!!");
            break;
        }
    }   
}

//search for a client by his nickname 
int search_a_client(Server *sev,std :: string NickName)
{
    std::map<int,Client>::iterator it = sev->clients.begin();
    for(; it != sev->clients.end();it++)
    {
        if(it->second.nickname.compare(NickName) == 0)
        {
            return(it->second.fd);
        }
    }
    return(0);
}

// SYNTAXE SENDFILE FILENAME  RECIEVER
void send_file(Server *sev,std :: vector<std :: string> & commands,Client cl)
{
    std :: FILE  *FileName;
    int fd = search_a_client(sev,commands[2]);

    if(commands.size() < 3)
    {
        Server::sendMsg(cl, ERR_NEEDMOREPARAMS);
        return;
    }
    //open file both binary and text
    FileName = fopen(commands[1].c_str(),"rb"); 
    if(!FileName)
    {
        Server::sendMsg(cl,"ERROR FILETRANSFER : No Such file in your /DIR");
        return;
    }
    //if not found reciever 
    if(!fd)
    {
        Server::sendMsg(cl,"ERROR FILETRANSFER : No Such a client");
        return;
    }
    // creat object file and push it in client vector of files
    TFile fl(FileName,commands[1].c_str(),cl.nickname,commands[2].c_str());
    std::map<int,Client>::iterator rec = sev->clients.find(fd);
    rec->second.Files.push_back(fl);
}



// SYNTAXE : GETFILE FILENAME SENDER 
void get_file(Server *srv,std :: vector<std :: string> command,Client cl)
{
    if(command.size() < 3)
    {
        Server::sendMsg(cl,ERR_NEEDMOREPARAMS);
        return;
    }
    else if(command[1].empty())
    {
        Server::sendMsg(cl,"ERROR FILETRANSFER : FILENAME NOT FOUND\n");
        return;
    }
    // if c'ant find the sender of file
    else if(!search_a_client(srv,command[2]))
    {
        Server::sendMsg(cl,"ERROR FILETRANSFER : NO SUCH A CLIENT\n");
        return;
    }
    // if there is no files in client vector files
    else if(cl.Files.empty())
    {
        Server::sendMsg(cl,"ERROR FILETRANSFER : NO SUCH A FILE TO GET IT !!!\n");
    }
    // if there is no file from sender
    else if(!search_a_file(cl,command[2].c_str()))
    {
        Server::sendMsg(cl,"ERROR FILETRANSFER : NO SUCH A FILE TO GET IT FROM SENDER!!!\n");
    }
    else
    {
        // creat file in reciever /dir 
        creat_file(cl,command[2],command[1]);
    }
    
}

int  search_a_file(Client clt,std :: string sender)
{
    std :: vector<TFile>::iterator it = clt.Files.begin();

    for(; it != clt.Files.end();it++)
    {
        if(it->getSender().compare(sender) == 0)
        {
            return(1);
        }
    }
    return(0);
}

void creat_file(Client clt,std :: string sender,std :: string filename)
{
    int file_size;
    char *line = NULL;
    FILE * fd;
    std :: vector<TFile>::iterator it = clt.Files.begin();
    std::fstream myfile;

    for(; it != clt.Files.end();it++)
    {
        if(it->getSender().compare(sender) == 0)
        {
            fd = it->getstream();
        }   
    }
    //determine size of file
        int prev = ftell(fd);
        std::fseek(fd,0,SEEK_END);
        file_size = ftell(fd);
        std::fseek(fd,prev,SEEK_SET);
        try{
            line = new char[file_size];
        }
        catch(std::exception &e)
        {
            std :: cerr << e.what() << std :: endl;
        }
    // open the new file in client /dir
    myfile.open("transferd_" + filename,std::ios::out | std::ios::binary);
    if(myfile.is_open() == 0)
    {
        clt.sendMsg(clt,"C 'ant open file ");
        myfile.close();
        clt.Files.clear();
    }
    // read from sender file 
    int readbytes = fread(line,1,file_size,fd);
    if(readbytes == -1)
    {
        clt.sendMsg(clt,"C'ant read from file");
        myfile.close();
        clt.Files.clear();
    }
    myfile.write(line,file_size);
    myfile.close();
    clt.Files.clear();
    
}

void prv_msg(Server *srv,std::vector<std :: string>command,int id)
{
    int i = 0;
    std::map<int,Client>::iterator it = srv->clients.find(id);
    if(command.size() < 3)
    {
       Server::sendMsg(it->second,ERR_NEEDMOREPARAMS);
        return;
    }
    for(;command[i][0] != ':';i++)
    {
        if(!search_a_client(srv,command[i]))
        {
           Server::sendMsg(it->second,command[i] + ERR_NOSUCHNICK);
        }
    }
}
