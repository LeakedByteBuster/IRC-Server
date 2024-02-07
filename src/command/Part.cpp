// #include "Server.hpp"



// std::vector<std::string> parsePart(Client &clt, std::vector<std::string> &command)
// {
//     std::vector<std::string>   tokens;
//     std::vector<std::string>                            splited;
//     std::string                                         error;

//     splited = splitByValue(command[0], ',');
//     if (splited.empty()) {
//         Server::sendMsg(clt, JOIN_ERR(Channel(command[0]), clt, ERR_BADCHANMASK));
//         return (tokens);
//     }
    
//     for (size_t i = 0; i < splited.size(); i++) {
//         if (!isChannelNameCorrect(splited[i]) || splited[i].size() > MAX_CHANNEL_NAME_LEN + 1)
//          {
//             if (i != 0)
//                 error.append("\r\n");
//             error.append(JOIN_ERR( Channel(splited[i]), clt, ERR_BADCHANMASK));
//             continue ;
//         }
//         tokens.push_back(splited[i]);
//     }
//      if (!error.empty()) {
//         Server::sendMsg( clt, error );
//     }
//     return (tokens);
// }
// void part (Client &clt, std::vector<std::string> &command)
// {
//     std::vector<std::string>   tokens = parsePart (clt ,command);
//     std::string reason = reasonArg (command,2);
//     for (size_t i = 0 ; i < tokens.size () ; ++i)
//     {
//         if 
//     }
// }