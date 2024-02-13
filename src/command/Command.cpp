#include "Server.hpp"
// #include "Bot.hpp"
#include "registrationCommands.hpp"
#include "FTP.hpp"

// check command if it's valide and exucte it
void execute_commmand(std::map<int, Client> &clients, std::vector<std::string> &commands, int id)
{
    int res = 0;
    if (!commands.empty())
    {
        res =  whichCommand(commands[0]);
        switch (res)
        {
        case SENDFILE:
            send_file(clients, commands, clients[id]);
            break;

        case GETFILE:
            get_file(clients, commands, clients[id]);
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
                parseNick(clients, clients[id], buff);
            }
            catch (std::exception &e) { }
            break;

        case PASS_USER:
            Server::sendMsg(clients[id], _ERR(clients[id].nickname, ERR_ALREADYREGISTRED));
            break;

        case PRVMSG:
            prv_msg(commands, clients[id],clients);
            break ;

        case PONG: // ignore PONG
            break;
        
        
        case JOIN: // join Command
            join(clients[id], commands);
            break;

        case KICK: // kick command
            Operator::kick(clients[id], commands,clients);    
            break;


        case TOPIC: // TOPIC command
            Operator::topic(clients[id], commands);    
            break;

        case INVITE: // TOPIC command
            Operator::invite(clients[id], commands,clients);    
            break;
        

        case MODE:
            Operator::mode(clients[id], commands);    
            break;

        case QUIT:
            Client_Quit(clients[id], commands,clients);
             break;

        case PART:
                Part_client(clients[id],commands);
                break;

        default:
            Server::sendMsg(clients[id], _ERR(clients[id].nickname, ERR_UNKNOWNCOMMAND));
            break;
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
