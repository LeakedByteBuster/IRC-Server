#include "Command.hpp"

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
    }
    
}

void send_file(Server *sev,std :: vector<std :: string> & commands,int id)
{
    std::fstream FileName;
    std::map<int,Client>::iterator it = sev->clients.find(id);
    if(commands.size() < 4)
    {
        it->second.sendMsg(id,ERR_NEEDMOREPARAMS);
    }
    if(FileName.open(commands[1].c_str(),std:: ios::in))
}
