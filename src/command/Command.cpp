#include "Command.hpp"
#include "Server.hpp"
#include "Bot.hpp"
#include "registrationCommands.hpp"

// split command into vector of string to check it
std ::vector<std ::string> HandleIncomingMsg(std ::vector<std ::string> &commands, std ::string msg)
{
    std::stringstream ss(msg);
    std ::string token;
    std::string tmp;

    std ::getline(ss, token, '\n');
    std::stringstream parser(token);
    while (getline(parser, tmp, ' '))
    {
        commands.push_back(tmp);
    }
    return (commands);
}

// check command if it's valide and exucte it
void execute_commmand(std::map<int,Client> &clients, std ::vector<std ::string> &commands, int id,std::map<int,channel> &channels)
{
    int res = 0;

    if (!commands.empty())
    {

        std ::string first_argument = commands[0];
        std::map<int, Client>::iterator it = clients.find(id);

        if(it == clients.end())
        {
            return;
        }
        switch (res)
        {
        case 1:
            send_file(clients, commands, it->second);
            break;

        case 2:
            get_file(clients, commands, it->second);
            break;

        case 3:
            try
            {
                std::string buff;
                for (unsigned long i = 0; i < commands.size(); i++)
                {
                    if (i != 0)
                        buff = buff.append(" ");
                    buff = buff.append(commands[i]);
                }
                parseNick(clients, it->second, buff);
            }
            catch (std::exception &e)
            {
            }
            break;

        case 4:
            Server::sendMsg(it->second, LogError::getError(it->second.nickname, LogError::ERR_ALREADYREGISTRED));
            break;

        case 5:
            prv_msg(channels, commands, it->second,clients);
            break;

        case 9:
            // ignore PONG
            break;

        case 10:
            // bot
            std::cout << commands[0] << std::endl;
            Server::sendMsg(it->second, Bot::botExecuter(commands[0]));
            break;
        
        default:
            Server::sendMsg(it->second, LogError::getError(it->second.nickname, LogError::ERR_UNKNOWNCOMMAND));
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
void send_file(std::map<int,Client> clients, std ::vector<std ::string> &commands, Client cl)
{
    std ::FILE *FileName;

    if (commands.size() < 3)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NEEDMOREPARAM));
        return;
    }
    // open file both binary and text
    FileName = fopen(commands[1].c_str(), "rb");
    if (!FileName)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHFILE));
        return;
    }
    // if not found reciever
    int fd = search_a_client(clients, commands[2]);
    if (!fd)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHNICK));
        return;
    }
    // creat object file and push it in client vector of files
    TFile fl(FileName, commands[1].c_str(), cl.nickname, commands[2].c_str());
    std::map<int, Client>::iterator rec = clients.find(fd);
    rec->second.Files.push_back(fl);
    cl.sendMsg(cl, getDownMsg());
}

// SYNTAXE : GETFILE FILENAME SENDER
void get_file(std::map<int,Client> clients, std ::vector<std ::string> command, Client cl)
{
    if (command.size() != 3)
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NEEDMOREPARAM));
        return;
    }
    else if (command[1].empty())
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHFILENAME));
        return;
    }
    // if c'ant find the sender of file
    else if (!search_a_client(clients, command[2]))
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHNICK));
        return;
    }
    // if there is no files in client vector files
    else if (cl.Files.empty())
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOSUCHFILENAME));
        return;
    }
    // if there is no file from sender
    else if (!search_a_file(cl, command[2].c_str()))
    {
        Server::sendMsg(cl, LogError::getError(cl.nickname, LogError::ERR_NOFILEFROMSENDER));
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

int search_msg(std::vector<std::string> command)
{
    int i = 0;
    std::vector<std::string>::iterator it = command.begin();
    for(; it != command.end();it++)
    {
        if(it->find(":") != std::string::npos)
        {
            return(i);
        }
        i++;
    }
    return(0);
}

void prv_msg(std::map<int,channel> &channels, std::vector<std ::string> command, Client clt,std::map<int,Client> clients)
{
    size_t position = search_msg(command);
    if (command.size() < 3)
    {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NEEDMOREPARAM));
        return;
    }
    else if (position == 0)
    {
        Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NOTEXTTOSEND));
        return;
    }
    check_targets(channels,command,clt,position,clients);
}

std :: string compile_msg(std::vector<std::string> commands,int position)
{
    size_t i = position;
    std :: string msg;

    for(; i < commands.size();i++)
    {
        if(i != commands.size() - 1)
            msg = msg.append(commands[i] + " ");
        else
            msg = msg.append(commands[i]);
    }
    return(msg);
}


void check_targets(std::map<int,channel> channels, std::vector<std::string> command, Client clt,size_t position,std::map<int,Client> clients)
{
    std :: string msg = compile_msg(command,position);
    for(size_t i = 1; i < position;i++)
    {
        if(command[i].find('#') == 0)
        {
            int id = search_in_channels(channels,command[i],clt);
            if(id)
            {
                std::map<int,channel>::iterator it = channels.find(id);
                (void) it;

                // send Massege to channel
            }
        }
        else
        {
            int id = search_a_client(clients,command[i]);
            if(id)
            {
                std::map<int,Client>::iterator it = clients.find(id);
                sendPrvmsg(clt,msg,it->second);
                // send Massege to client;
            }
            else
            {
                Server::sendMsg(clt, LogError::getError(clt.nickname, LogError::ERR_NOSUCHNICK));
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

void sendPrvmsg(Client sender,std::string str,Client recv)
{
    std::string msg = sender.nickname + " PRVMSG " + recv.nickname;
    msg.append(" " + str);
    Server::sendMsg(recv,msg);
    LogError::rplAwayMsg(sender,msg); 
}