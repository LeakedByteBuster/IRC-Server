#include "Server.hpp"

/*

------------ 'l' MODE
MODE #1997 -l [ignores it silently, if not already set]
MODE #986 +l s s s s s [ignores it silently]
MODE #986 +l 0 [ignores it silently]

MODE #986 -l scc cs ca c aca
    :apo98s!~apo@197.230.30.146 MODE #986 -l 
MODE #986 +l 11
    :apo98s!~apo@197.230.30.146 MODE #986 +l 11
MODE #986 +l 11ssdsd6++
    :apo98s!~apo@197.230.30.146 MODE #986 +l 11
MODE #986 +l 2147483647
    :apo98s!~apo@197.230.30.146 MODE #986 +l 2147483647

MODE #986 +l
    :mercury.libera.chat 461 apo98s MODE :Not enough parameters

------------- 'i' MODE
MODE #1997 -i [ignores it silently, if not already set]

MODE #1997 +i
    :Jack!~hoba@197.230.30.146 MODE #1997 +i 
MODE #986 -i jk hkjhk jh
    :apo98s!~apo@197.230.30.146 MODE #986 -i 

------------- 'o' MODE
MODE #986  +o 02 [ignores it silently]
MODE #986 -o 02 [ignores it silently]

MODE #986 +o apo98 
    :apo98s!~apo@197.230.30.146 MODE #986 +o apo98
MODE #986 -o apo98
    :apo98s!~apo@197.230.30.146 MODE #986 -o apo98

MODE #1997 -o 
    :tantalum.libera.chat 461 Jack MODE :Not enough parameters
MODE #1997 +o 
    :tantalum.libera.chat 461 Jack MODE :Not enough parameters
MODE #joi0556 +o dsad
    Error(401): dsad No such nick/channel

-------------  'k' MODE

MODE #1997 +k [ignores it silently]
MODE #1997 -k [ignores it silently]

MODE #1997 +k 0
    :Doug007!~hoba@197.230.30.146 MODE #1997 +k 0
MODE #4989 -k dsajkdhasjdhasjkdhasjdhasdkahsd
    :apo98s!~apo@197.230.30.146 MODE #4989 -k *

-------------  't' MODE
mode #77 +t hello every one [ignores it silently, if already set]


---------------------------------------------------------
OTHERS :
--------

MODE #1997    
    :tantalum.libera.chat 324 Jack #1997 +Cinst
    :tantalum.libera.chat 329 Jack #1997 1707369961
MODE #986
    :mercury.libera.chat 324 apo98s #986 +lk 11 apo98
    :mercury.libera.chat 329 apo98s #986 1707441174

MODE #1997 -i+o Alfredo fdsfsd dfsfs
    :Jack!~hoba@197.230.30.146 MODE #1997 +o Alfredo


MODE #1997 -oi+k 0 Doug007
    :Doug007!~hoba@197.230.30.146 MODE #1997 +k Doug007

 ------ERRORS--------ERRORS---------ERRORS------------ 

mode
    :zirconium.libera.chat 461 apo9 MODE :Not enough parameters

MODE #1997 -i
    :lead.libera.chat 482 Alfredo #1997 :You're not a channel operator

MODE #1997 -Cn+io*
    :tantalum.libera.chat 461 Doug007 MODE :Not enough parameters

MODE #1997 -Cn+io* Doug007
    :tantalum.libera.chat 472 Doug007 * :is an unknown MODE char to me

MODE #1996
    :tantalum.libera.chat 403 Doug007 #1996 :No such channel

mode #77 -ok jkhfdskjfds 
    :zirconium.libera.chat 401 apo9 jkhfdskjfds :No such nick/channel
*/


// expect arg: +o, +l, +k
// expect no arg: -l, -k, -i, -t, +t

// MODE #1997    
//     :tantalum.libera.chat 324 Jack #1997 +Cinst
//     :tantalum.libera.chat 329 Jack #1997 1707369961

static void    listChannelModes(const Channel &ch, Client clt)
{
    std::stringstream   ss;
    std::string         token;
    std::string         msg = replyPrefix(ch, clt, "324");
    ss << time(NULL);
    ss >> token;
    msg.append(" " + ch.getModeString() + "\r\n");
    msg.append(replyPrefix(ch, clt, "329") + " " + token);
    Server::sendMsg(clt, msg);
}

typedef std::pair<Message::error_t, bool>   error_pair;

void    Operator::mode(Client clt, std::vector<std::string> args)
{
    try {
        if (args.size() <= 1) 
            throw error_pair(ERR_NEEDMOREPARAMS, 1);
        
        std::__1::map<std::__1::string, Channel>::iterator it;
        it = Server::ChannelsInServer.find(args[1]);
        if (it == Server::ChannelsInServer.end())
            throw error_pair(ERR_NOSUCHCHANNEL, 0);

        if (args.size() == 2) {
            listChannelModes(it->second, clt);
            return ;
        }

    } catch (error_pair errorNum) {
        if (errorNum.second == 1)
            Server::sendMsg(clt, _ERR(clt.nickname, errorNum.first));
        else
            Server::sendMsg(clt, JOIN_ERR(Channel(args[1]), clt, errorNum.first));
    }

    return ;
}
