#include "Server.hpp"

typedef std::pair<Message::error_t, bool>                                       error_pair;
typedef std::pair<Message::error_t, std::string>                                logError;
typedef std::vector< std::pair< std::pair< std::string, std::string >, bool > > modesList;

typedef struct s_Modes {
    //  vec<pair < pair < mode, param >, minus | plus > > || (bool == '+') = 1 / (bool == '-') = 0
    modesList   toExecute;
}   t_Modes;

/* -------------------------------------------------------------------------- */
/*                                   Utils                                    */
/* -------------------------------------------------------------------------- */

static inline Channel  &findChannel(std::string name)
{
    std::__1::map<std::__1::string, Channel>::iterator it;
    it = Server::ChannelsInServer.find(name);
    if (it == Server::ChannelsInServer.end())
        throw error_pair(ERR_NOSUCHCHANNEL, 0);
    return (it->second);
}

static inline bool isValidChar(char c)
{
    static const   std::string modes = "ioklt+-";
    return ( ( modes.find(c) != std::string::npos ) ? 1 : 0 );
}

static std::string  appendModeArgs(std::vector<std::string> vec) 
{
    std::string args;

    for (size_t i = 3; i < vec.size(); i++) {
        if (i != vec[i].size() - 1) {
            args.append(vec[i] + " ");
            continue ;
        }
        args.append(vec[i] + " ");
    }
    return (args);
}

static inline void    putSign(std::string &res, bool sign, bool &pSign, bool &mSign)
{
    if (sign == 1 && !pSign) {
        res.append("+");
        pSign = 1;
        mSign = 0;
    } else if (sign == 0 && !mSign) {
        res.append("-");
        pSign = 0;
        mSign = 1;
    }
}

/* -------------------------------------------------------------------------- */
/*                                  Parsing                                   */
/* -------------------------------------------------------------------------- */

/*
    +l : have a value [ERR_NEEDMOREPARAM]
    +o : have a value [ERR_NEEDMOREPARAM]
    +k : have a value [No error if not supplied]
    +t : does not have a value
    +i : does not have a value

    -k : have a value [ignore it]
    -o : have a value [ERR_NEEDMOREPARAM]
    -l : does not have a value
    -t : does not have a value
    -i : does not have a value
*/
static std::vector<std::string>    parseModeString(std::string ms, std::vector<std::string> args)
{
    std::string                 err;
    std::vector<std::string>    buff = splitByValue(ms, '+');
    size_t                      argSize = (args.size() > 0) ? args.size() : 0;
    bool                        minus; // used to check if a minus char is seen

    for (size_t i = 0; i < buff.size() ; i++) {
        minus = 0;
        for (size_t j = 0; j < buff[i].size(); j++) {
            if (! isValidChar(buff[i][j]) ) {
                throw logError(ERR_UNKNOWNMODE, (err = buff[i][j]));
            }
            if (buff[i][j] == '-') {
                 minus = 1; 
                 continue ;
            }
            if ( ( (buff[i][j] == 'k') || (buff[i][j] == 'o') || (buff[i][j] == 'l') ) && !minus ) {
                if (argSize > 0) {
                    argSize--;
                    continue ;
                }
                throw error_pair(ERR_NEEDMOREPARAMS, 1);
            }
            if ( ( (buff[i][j] == 'k') || (buff[i][j] == 'o') ) && minus ) {
                if (argSize > 0) {
                    argSize--;
                    continue ;
                }
                throw error_pair(ERR_NEEDMOREPARAMS, 1);
            }
        }
    }
    return (buff);
}

/* -------------------------------------------------------------------------- */
/*                            Storing Mode String                             */
/* -------------------------------------------------------------------------- */

static void insertMinusModes(const std::vector<std::string> &param, t_Modes &modes, std::vector<std::string> mString, size_t &n)
{
    std::string toPush;

    for (size_t i = 0; i < mString.size(); i++) {

        for (size_t j = 0; j < mString[i].size(); j++) {
            toPush = mString[i][j];
            if ( toPush.compare("-") != 0 ) { // if it's a '-' skip it
                if ( !toPush.compare("o") || !toPush.compare("k")) {
                    // modes.minusModes.push_back(std::make_pair(toPush, param[n]));
                    modes.toExecute.push_back( make_pair( make_pair(toPush, param[n]), 0 ) );
                    n++;
                    continue ;
                }
                // modes.minusModes.push_back(std::make_pair(toPush, ""));
                modes.toExecute.push_back( make_pair( make_pair(toPush, ""), 0 ) );
            }
        }
    }
    return ;
}

static void insertPlusModes(const std::vector<std::string> &param, t_Modes &modes, std::string mString, size_t &n)
{
    std::string toPush;

    for (size_t j = 0; j < mString.size(); j++) {
        toPush = mString[j];
        if ( !toPush.compare("o") || !toPush.compare("l") || !toPush.compare("k")) {
            // modes.plusModes.push_back(std::make_pair(toPush, param[n]));
            modes.toExecute.push_back( make_pair( make_pair(toPush, param[n]), 1 ) );
            n++;
            continue ;
        }
        // modes.plusModes.push_back(std::make_pair(toPush, ""));
        modes.toExecute.push_back( make_pair( make_pair(toPush, ""), 1 ) );
    }
    return ;
}

static t_Modes   separateModes(std::vector<std::string> mString, std::vector<std::string> param)
{
    t_Modes modesVecs; // holds vector of plusModes and minusModes
    size_t  n = 0; // used for param arguments

    for (size_t i = 0; i < mString.size(); i++) {

        size_t  pos = 0; // used for substr in case there's a '-' char

        /* Insert all characters in mString[i] as plus modes. No '-' char is found  */
        if (( pos = mString[i].find('-') ) == std::string::npos) {
            insertPlusModes(param, modesVecs, mString[i], n);
            continue ;
        }
        /* Insert plus and minus modes each in its proper variable in t_modes var */
        std::vector<std::string>    modesStr = splitByValue(mString[i], '-'); // modeStr[0] are plus modes, rest is minus modes
        if (modesStr.size() == 0) { continue ; } // no modes were given
        if (mString[i][0] != '-') {
            insertPlusModes(param, modesVecs, modesStr[0], n); // insert plus modes
            modesStr.erase(modesStr.begin()); // deletes modesStr[0] which is a string of plus modes
        }
        insertMinusModes( param, modesVecs, modesStr, n ); // insert minus modes in vector modesStr
    }
    return (modesVecs);
}

/* -------------------------------------------------------------------------- */
/*                               Modes Methods                                */
/* -------------------------------------------------------------------------- */

static bool mode_O(Channel &ch, std::string arg, const bool &sign, const Client &clt)
{
    int cltFd = search_a_client(ch.clientsInChannel, arg);
    if (cltFd == 0) {
        Server::sendMsg(clt, _ERR(arg, ERR_NOSUCHNICK));
        return (0); 
    }
    if ((sign && (ch.clientsInChannel[cltFd].isOperator == 1))
        || (!sign && (ch.clientsInChannel[cltFd].isOperator == 0))) {
        return (0);
    }
    (sign == 1) ? ch.clientsInChannel[cltFd].isOperator = 1 : ch.clientsInChannel[cltFd].isOperator = 0;
    return (1);
}

static bool mode_K(Channel &ch, std::string key, const bool &sign)
{
    if ((sign && (ch.isKey == 1)) || (!sign && (ch.isKey == 0)))
        return (0);

    if (sign == 1) {
        ch.isKey = 1;
        ch.setKey(key);
        return (1);
    }
    ch.isKey = 0;
    ch.setKey("");
    return (1);
}

static bool mode_L(Channel &ch, std::string param, const bool &sign)
{
    if ((sign && (ch.isUsersLimit == 1)) || (!sign && (ch.isUsersLimit == 0)))
        return (0);

    if (!param.empty()) {
        char    *endptr = NULL;
        long    res = std::strtol(param.data(), &endptr, 10);
        if ((*endptr != '\0') || (errno == ERANGE) || res < 1 || res > USERS_CHANNEL_LIMIT) {
            return (0);
        }
        ch.isUsersLimit = 1;
        ch.usersLimit = res;
        return (1);
    }

    ch.isUsersLimit = 0;
    return (1);
}

static bool mode_T(Channel &ch, const bool &sign)
{
    if ((sign && (ch.isTopic == 1)) || (!sign && (ch.isTopic == 0)))
        return (0);
    (sign == 1) ? ch.isTopic = 1 : ch.isTopic = 0;
    return (1);
}

static bool mode_I(Channel &ch, const bool &sign)
{
    if ((sign && (ch.isInviteOnly == 1)) || (!sign && (ch.isInviteOnly == 0)))
        return (0);
    (sign == 1) ? ch.isInviteOnly = 1 : ch.isInviteOnly = 0;
    return (1);
}

/* -------------------------------------------------------------------------- */
/*                                    MODE                                    */
/* -------------------------------------------------------------------------- */

static inline void    listChannelModes(const Channel &ch, Client clt)
{
    std::stringstream   ss;
    std::string         token;
    std::string         msg;
    
    msg = replyPrefix(ch, clt, "324");
    ss << time(NULL);
    ss >> token;
    msg.append(" " + ch.getModeString() + "\r\n");
    msg.append(replyPrefix(ch, clt, "329") + " " + token);
    Server::sendMsg(clt, msg);
}

static std::string    setModes(Client &clt, const modesList &list, Channel &ch)
{
    std::string res; // string used as a return value of modes set
    std::string resArgs(" "); // used to holds param of modes (appended with res at the end)
    std::string mode; // current char in while
    std::string param; // current correspondant param of the mode (if any)
    bool        sign; // sign of the current char
    bool        pSign = 0; // used to append + to res
    bool        mSign = 0; // used to append - to res

    for (size_t i = 0; i < list.size(); i++) {
        mode = list[i].first.first;
        param = list[i].first.second;
        sign = list[i].second;


        if ((mode.compare("i") == 0) && mode_I(ch, sign)) {
            putSign(res, sign, pSign, mSign);
            res.append(mode);
            continue ;
        }
        if ((mode.compare("t") == 0) && mode_T(ch, sign)) {
            putSign(res, sign, pSign, mSign);
            res.append(mode);
            continue ;
        }
        if ((mode.compare("l") == 0) && mode_L(ch, param, sign)) {
            putSign(res, sign, pSign, mSign);
            res.append(mode);
            if (sign == 1)
                resArgs.append(" " + param);
            continue ;
        }
        if ((mode.compare("k") == 0) && mode_K(ch, param, sign)) {
            putSign(res, sign, pSign, mSign);
            res.append(mode);
            (sign == 1) ? resArgs.append(" " + param) : resArgs.append(" *");
            continue ;
        }
        if ((mode.compare("o") == 0) && mode_O(ch, param, sign, clt)) {
            putSign(res, sign, pSign, mSign);
            res.append(mode);
            resArgs.append(" " + param);
            continue ;
        }
    }
    if (resArgs.size() > 1) // because it's initialized with space 0x20
        res.append(resArgs);
    return (res);
}

/*
    construct mode string using a bool as return in each mode function
*/
void    Operator::mode(Client &clt, const std::vector<std::string> &args)
{
    // std::vector< std::pair<std::string, std::string> > modes;
    try
    {
        if (args.size() <= 1) 
            throw error_pair(ERR_NEEDMOREPARAMS, 1);
        Channel &ch = findChannel(args[1]);
        if (args.size() == 2) { listChannelModes(ch, clt); return ; }
        // maybe return an error : not in channel
        int cltFd = search_a_client(ch.clientsInChannel, clt.nickname); if (cltFd == 0) { return ; }

        if (ch.clientsInChannel[clt.fd].isOperator == 0) { throw error_pair(ERR_CHANOPRIVSNEEDED, 1) ; }

        std::vector<std::string>    param = splitBySpace(appendModeArgs(args));
        std::vector<std::string>    modeStrings = parseModeString(args[2], param);

        t_Modes modes;

        modes = separateModes(modeStrings, param);
        if (modes.toExecute.size() == 0) { return ; } // empty mode string is given

        std::string res = setModes(clt, modes.toExecute, ch);
        if (!res.empty()) {
            std::string msg = commandReply(ch, clt, "MODE", TYPE_USER) + " " + res;
            Server::sendMsg(ch, msg);
        }

    } catch (error_pair error) {
        if (error.second == 1)
            Server::sendMsg(clt, _ERR(clt.nickname, error.first));
        else
            Server::sendMsg(clt, JOIN_ERR(Channel(args[1]), clt, error.first));
    } catch (logError err) {
        Server::sendMsg(clt, JOIN_ERR(Channel(err.second), clt, err.first));
    }

    return ;
}
