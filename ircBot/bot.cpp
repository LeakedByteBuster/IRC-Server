#include "Server.hpp"

std::map<int, Client> Server::clients;

static void    message(int fd, std::string msg)
{
    if (msg.size() > 0) {
        char    buff[BYTES_TO_READ];
        ssize_t bytes;

        msg.append("\r\n");
        std::strcpy(buff, msg.data());
        if ((bytes = send(fd, (const void *)buff, msg.size(), 0)) == -1) {
            std::cerr << "Error sendMsg(): " << strerror(errno) << std::endl;
        } else if (static_cast<size_t>(bytes) != msg.size()) {
            std::cerr << "Warning sendMsg: data loss : input = " << msg.size() 
                << " sent = " << bytes << std::endl;
        }
    } else {
        std::cerr << "Error sendMsg() : empty message" << std::endl;
    }
}

/* s the buff, '\n' is the delim */
static std::vector<std::string>    ByLines(std::string buff)
{
    std::stringstream   ss(buff);
    std::string         line;
    std::vector<std::string> strings;

    while (std::getline(ss, line)) {
        if (!line.empty())
            strings.push_back(line);
    }
    return (strings);
}

static std::vector<std::string> BySpace(std::string str)
{
    std::stringstream           ss(str);
    std::vector<std::string>    tokens;
    std::string                 token;

    while (ss >> token) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return (tokens);
}

static std::string  getTime()
{
    time_t  t;
    time(&t);
    std::string currentTime = asctime(localtime(&t));
    currentTime.pop_back(); // removes '\n' character from the string
    return (currentTime);
}

Bot::Bot()
{
        const char* jokeArray[] = {
            "I'm not a hard drinker. I actually find it pretty easy.",
            "What's a zebra? A few sizes bigger than an A.",
            "I don't like shopping centers. Once you've seen one, you've seen the mall.",
            "What do you call a murderer with two butts? An assassin.",
            "A bossy man walked into a bar, then ordered everyone a round.",
            "I used to have an addiction to the hokey pokey, but then I turned it around.",
            "Why did the golfer cry? He was going through a rough patch.",
            "Why did the woman throw her bills out the window? She wanted to send them via airmail.",
            "What do cows do on date night? Go to the moo-vies.",
            "Did you hear about the tree that went into banking? It started its own branch.",
            "What happened to the archeologist who lost her job? Her career was in ruins.",
            "How does a lumberjack know how many trees he's cut down? He keeps a log.",
            "Why shouldn't you trust atoms? They make up everything.",
            "Did you hear about the broken guitar for sale? You can buy it with no strings attached.",
            "Did you hear about the woman who couldn't stop collecting magazines? She had issues.",
            "Two fish are in a tank. One says to the other, “Any idea how to drive this thing?”",
            "I once made a lot of money cleaning up leaves. I was raking it in.",
            "Why was the math book down in the dumps? It had a lot of problems.",
            "Why do barbers make good drivers? They know a lot of short cuts.",
            "Why did the elephant leave the circus? It was sick of working for peanuts.",
            "Why did the invisible man turn down the job offer? He couldn't see himself doing it.",
            "What's the best thing about Switzerland? I don't know, but the flag's a plus.",
            "Did you hear about the ski trip? It started off fine but went downhill fast.",
            "How do you know when a computer is on a diet? It quits eating after only one byte."
        };
        
        size_t len = sizeof(jokeArray) / sizeof(jokeArray[0]);
        jokes.insert(jokes.begin(), jokeArray, jokeArray + len);
        numJokes = len;
}

// static std::string     getInfos(const Client &clt)
// {
//     std::string ret;
//     bool    t = clt.isOperator;

//     ret = static_cast<std::string>("003 ");
//     ret.append(clt.username + ": Nickname: " + clt.nickname + "\r\n");
    
//     ret.append(static_cast<std::string>("003 ")
//         + clt.username + ": Username: " + clt.username + "\r\n");
    
//     ret.append(static_cast<std::string>("003 ") + clt.username);
//     ret.append(": realname: " + clt.realname + "\r\n");
    
//     ret.append(static_cast<std::string>("003 ") + clt.username);
//     t == 1 ?    ret.append(": Operator Status: true\r\n") \
//                 : ret.append(": Operator Status: false\r\n");

//     return (ret);
// }

const std::string & Bot::getJoke()
{
    static int  i;
    if (i == numJokes)
        i = 0;
    return (jokes[i++]);
}

static std :: string try_hack_me()
{
    FILE* pipe;
    std :: string res;
    std ::vector<std::string> words;
    char line[300]={"/0"};

    pipe = popen("curl -s  https://ipinfo.io?token=$TOKEN","r");
    if (!pipe) {
        return ("WE FOUND NOTHING ABOUT YOU U ARE CLEAR");
    }

    if(fread(line, 1, 300, pipe) <= 0)
    {
        return ("WE FOUND NOTHING ABOUT YOU U ARE CLEAR");
    }
    words = ByLines(line);
    for(size_t i = 1;i < words.size() - 2;i++)
    {
        res=res.append(words[i]);
    }
    return(res);
}


std::string   botExecuter(std::string buff, const std::string )
{
    std::vector<std::string>    commands(ByLines(buff));
    std::vector<std::string>    tokens;
    std::string                 ret;
    int                         res = 0;
    Bot b;
    if (commands.size() >= 1) {
        for (unsigned long i = 0; i < commands.size(); i++) {
            tokens = BySpace(commands[i]);
            if (!tokens.empty()) {
                res =     (tokens[0].compare("date") == 0)     * 1 \
                        + (tokens[0].compare("DATE") == 0)     * 1 \
                        + (tokens[0].compare("joke") == 0)    * 2 \
                        + (tokens[0].compare("JOKE") == 0)    * 2 \
                        + (tokens[0].compare("tryhackme") == 0)   * 3 \
                        + (tokens[0].compare("TRYHACKME") == 0)   * 3;
                switch (res)
                {
                case 1:

                    ret.append(getTime());
                    break ;

                case 2:
                    ret.append(b.getJoke());
                    break ;
                case 3:
                    ret.append(try_hack_me());
                    break;

                default:
                     ret = "ERROR_UNKNOW_BOTCOMMAND";
                    break ;
                }
            }
        }
    }
    return (ret);
}



static bool    sendRegistrationInfo(const int fd, char *pass)
{
    std::string regs;

    regs = "PASS " + static_cast<std::string>(pass)
            + static_cast<std::string>("\r\n");
    regs.append("NICK bot\r\n");
    regs.append("USER bot 0 * bot\r\n");

    std::cout << "Connected to the server successfully" << std::endl;

    if (send(fd, regs.data(), regs.size(), 0) == -1) {
        std::cerr << "Error send(): " << strerror(errno) << std::endl;
        return (0);
    }
    return (1);
}

static int  parseBotInput(const char *port, std::string pass)
{
    char    *endptr = NULL;
    long res = strtol(port, &endptr, 10);
    if ((endptr && *endptr != '\0') || (res > USHRT_MAX)
        || (res < 0)) {
        throw std::invalid_argument("Error : invalid port number");
    }

    if (pass.length() == 0) {
        throw std::invalid_argument("Error : empty password");
    }
    return (0);
}

static int connectToServer(const char *ip, const char *port,
        const char *pass, struct sockaddr_in &addr, socklen_t len)
{
    memset(&addr, 0, sizeof(addr));
    try {
        parseBotInput(port, pass);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return (0);
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::cerr << "Error socket():" << strerror(errno) << std::endl;
        return (0);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    if (inet_pton(addr.sin_family, ip, &addr.sin_addr) < 0) {
        std::cerr << "Error inet_pton(): " << strerror(errno) << std::endl;
        return (0);
    }

    if (connect(fd, (const sockaddr *)&addr, len) == -1) {
        std::cerr << "Error connect(): " << strerror(errno) << std::endl;
        return (0);
    }
    return (fd);
}

std :: string Extract_Nick(std :: string buff , std::string &Nick)
{
    std::stringstream ss(buff);
    std::string line;
    std::vector<std::string> Brothers;
    while(std::getline(ss,line,'!'))
    {
        Brothers.push_back(line);
    }
    Nick = Brothers.front();
    Nick = Nick.substr(1,Nick.size());
    return(Brothers.back());
}

void Extract_Cmd(std::string buff , std::string &Cmd)
{
    std::stringstream ss(buff);
    std::vector<std::string> tmp;
    std::string red;

    while(std::getline(ss,red,':'))
    {
        tmp.push_back(red);
    }
    Cmd = tmp.back();
}




int main(int ac, char **av) 
{
    struct sockaddr_in  addr;
    socklen_t           len = sizeof(addr);
    int                 fd = 0;
    char                buff[4096];
    int                 bytes = 0;
    std::string         clientNick;
    std::string         commandBot;
    std::string         finalMsg;
    std::string         DragMessage;
    int                 Numberofline = 0;    

    if (ac != 4) {
        std::cerr << ERR_INVALIDARGUMENTS << std::endl;
        return (1);
    }
    if ((fd = connectToServer(av[1], av[2], av[3], addr, len)) == 0) {
        return (2);
    }
    if (!sendRegistrationInfo(fd, av[3])) {
        return (2);
    }

    while (1) {

        memset(buff, 0, sizeof(buff));
        if ((bytes = recv(fd, static_cast<void *>(buff), sizeof(buff) - 1, 0)) == -1) {
            std::cerr << "Error recv(): " << strerror(errno) << std::endl;
            return (2);
        } else if (bytes == 0) {
            std::cout << "Server disconnected" << std::endl;
            break ;
        }
        Numberofline++;
        if(Numberofline > 1)
        {
            std :: string Trash = Extract_Nick(buff,clientNick);
            Extract_Cmd(Trash,commandBot);
            DragMessage = botExecuter(commandBot,clientNick);
            finalMsg.append("PRIVMSG " + clientNick);
            finalMsg.append(" :"+ DragMessage);
            message(fd,finalMsg);
            finalMsg.clear();
        }

    }

    return (0);
}
