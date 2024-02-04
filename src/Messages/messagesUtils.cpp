#include "Server.hpp"

/* -------------------------------------------------------------------------- */
/*                              Messages Prefix                               */
/* -------------------------------------------------------------------------- */

// <':'><ircCamel.localhost> <Error Number> <Client Nickname>
std::string errorPrefix(const Client &clt, const std::string errNum)
{
    std::string buff(":");
    buff = SERVER_PREFIX + errNum + " " + clt.nickname;
    return (buff);
}

// nick!~user@hostname
std::string userPrefix(const Client &clt)
{
    std::string str(":");

    str = clt.nickname + " " + "!";
    str.append(clt.username + "@");
    str.append(inet_ntoa(clt.hints.sin_addr));
    return (str);
}

//  <nick!~user><@><hostname> <command> <channel name>
std::string replyCommandPrefix(const Channel &ch, const Client &clt, const std::string command, int user_or_server)
{
    std::string prefix = (user_or_server == TYPE_SERVER) ? SERVER_PREFIX : userPrefix(clt);

    std::string replyPrfx(
        prefix  +
        " "     +
        command +
        ch.name
    );
    return (replyPrfx);
}

// <':'>< ircCamel.localhost / <nick!~user><@><hostname>> <command> <channel name>
// prefix types : TYPE_SERVER, TYPE_USER
std::string commandReply(const Channel &ch, const Client &clt, std::string command, int prefixType)
{
    std::string rpl(":");
    std::string prefix(SERVER_PREFIX);

    if (prefixType == TYPE_USER) {
        prefix = userPrefix(clt);
    }

    rpl.append (
        prefix
        + " "
        + command
        + " "
        + ch.name
    );

    return (rpl);
}
