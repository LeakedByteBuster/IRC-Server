#include "Command.hpp"


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


// check command if it's valide and exucte it
void execute_commmand(Server *sev,std :: vector<std :: string> &commands,int id)
{
    if(!commands.empty())
    {
        std :: string first_argument = commands[0];
        std::map<int,Client>::iterator it = sev->clients.find(id);
        //int regi = it->second.isRegistred;
        if(it == sev->clients.end())
        {
            std :: cout << "No such a client\n";
        }
            if(!first_argument.compare("SEND"))
            {
                send_file(sev,commands,id);
            }
            else if(!first_argument.compare("PRIVMSG"))
            {
                send_msg(sev,commands,id);
            }
    }
    
}

//search for a client by his nickname 
int search_a_client(Server *sev,std :: string NickName)
{
    std::map<int,Client>::iterator it = sev->clients.begin();

    for(; it != sev->clients.begin();it++)
    {
        if(it->second.nickname.compare(NickName) == 0)
        {
            return(it->second.fd);
        }
    }
    return(0);
}

//send func by nickname 
void send_file(Server *sev,std :: vector<std :: string> & commands,int id)
{
    std :: fstream FileName;
    std :: string line;
    std::map<int,Client>::iterator it = sev->clients.find(id);
    if(commands.size() < 4)
    {
        it->second.sendMsg(id,ERR_NEEDMOREPARAMS);
        return;
    }
    FileName.open(commands[1].c_str(),std::ios::in);
    if(!FileName.is_open())
    {
        it->second.sendMsg(id,"No Such file in your /DIR\n");
        return;
    }
    if(commands[2].compare(it->second.nickname))
    {
        it->second.sendMsg(id,"the NickName u set for sender Don't match ur Nickname\n");
        return;
    }
    if(!search_a_client(sev,commands[3]))
    {
        it->second.sendMsg(id,"No Such a client\n");
        return;
    }
    
}

void send_msg(Server srv,std::vector<std::string> command,int id)
{
    
}