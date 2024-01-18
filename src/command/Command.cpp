#include "Command.hpp"
#include "Errors.hpp"
#include "registrationCommands.hpp"


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
                + (first_argument.compare("PRVMSG") == 0)   * 5 ;

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
        Server::sendMsg(cl, LogError::getError(cl.nickname,LogError::ERR_NEEDMOREPARAM));
        return;
    }
    //open file both binary and text
    FileName = fopen(commands[1].c_str(),"rb"); 
    if(!FileName)
    {
        Server::sendMsg(cl,LogError::getError(cl.nickname,LogError::ERR_NOSUCHFILE));
        return;
    }
    //if not found reciever 
    if(!fd)
    {
        Server::sendMsg(cl,LogError::getError(cl.nickname,LogError::ERR_NOSUCHNICK));
        return;
    }
    // creat object file and push it in client vector of files
    TFile fl(FileName,commands[1].c_str(),cl.nickname,commands[2].c_str());
    std::map<int,Client>::iterator rec = sev->clients.find(fd);
    rec->second.Files.push_back(fl);
    cl.sendMsg(cl,getDownMsg());
}



// SYNTAXE : GETFILE FILENAME SENDER 
void get_file(Server *srv,std :: vector<std :: string> command,Client cl)
{
    if(command.size() != 3)
    {
        Server::sendMsg(cl,LogError::getError(cl.nickname,LogError::ERR_NEEDMOREPARAM));
        return;
    }
    else if(command[1].empty())
    {
        Server::sendMsg(cl,LogError::getError(cl.nickname,LogError::ERR_NOSUCHFILENAME));
        return;
    }
    // if c'ant find the sender of file
    else if(!search_a_client(srv,command[2]))
    {
        Server::sendMsg(cl,LogError::getError(cl.nickname,LogError::ERR_NOSUCHNICK));
        return;
    }
    // if there is no files in client vector files
    else if(cl.Files.empty())
    {
        Server::sendMsg(cl,LogError::getError(cl.nickname,LogError::ERR_NOSUCHFILENAME));
        return;
    }
    // if there is no file from sender
    else if(!search_a_file(cl,command[2].c_str()))
    {
        Server::sendMsg(cl,LogError::getError(cl.nickname,LogError::ERR_NOFILEFROMSENDER));
        return;
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
        catch(std::bad_alloc &e)
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

// void prv_msg(Server *srv,std::vector<std :: string>command,Client clt)
// {
//     int i = 0;
//     if(command.size() < 3)
//     {
//         clt.sendMsg(clt,ERR_NEEDMOREPARAMS);
//         return;
//     }
//     for(;command[i][0] != ':';i++)
//     {
//         if(!search_a_client(srv,command[i]))
//         {
//             clt.sendMsg(clt,command[i] + ERR_NOSUCHNICK);
//         }
//     }
// }


const char * getDownMsg()
{

    const char * Down = 
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
    return(Down);
}