#include "Messages.hpp"

/* -------------------------------------------------------------------------- */
/*                       Static Error Messages Database                       */
/* -------------------------------------------------------------------------- */

// Sets the map in Message class to the specified static error message
void    Message::setErrorsDatabase()
{
    Message::MessageErrorNumber    errorNumbers[ERRORS_SIZE] = {
        Message::ERR_INCORRECT_PASS,    // 1
        Message::ERR_NONICKNAMEGIVEN,   // 2
        Message::ERR_ERRONEUSNICKNAME,  // 3
        Message::ERR_ERRONEUSUSERNAME,  // 4
        Message::ERR_NICKNAMEINUSE,     // 5
        Message::ERR_NEEDMOREPARAMS,    // 6
        Message::ERR_ALREADYREGISTRED,  // 7
        Message::ERR_UNKNOWNCOMMAND,    // 8
        Message::ERR_NOSUCHFILE,        // 9
        Message::ERR_NOSUCHNICK,        // 10
        Message::ERR_NOSUCHFILENAME,    // 11
        Message::ERR_NOFILEFROMSENDER,  // 12
        Message::ERR_NOTEXTTOSEND,      // 13
        Message::ERR_NOSUCHCHANNEL,     // 14
        Message::ERR_TOOMANYCHANNELS,   // 15
        Message::ERR_CHANNELISFULL,     // 16
        Message::ERR_INVITEONLYCHAN,    // 17
        Message::ERR_BADCHANMASK        // 18
        /* NOTICE: Change ERRORS_SIZE macro in Messages.hpp to the current size */
    };

    const char * staticErrorsString[] = {
          ":Password Incorrect"                 // 1
        , ":No nickname given"                  // 2
        , ":Erroneous nickname"                 // 3
        , ":Erroneous username"                 // 4
        , ":Nickname is already in use"         // 4
        , ":Not enough parameters"              // 6
        , ":You may not reregister"             // 7
        , ":Unknown command"                    // 8
        , ":No such a file in /DIR"             // 9
        , ":No such nick/channel"               // 10
        , ":/file name not found"               // 11
        , ":No file from sender"                // 12
        , ":No text to send"                    // 13
        , ":No such channel"                    // 14
        , ":You have joined too many channels"  // 15
        , ":Cannot join channel (+l)"           // 16
        , ":Cannot join channel (+i)"           // 17
        , ":Invalid channel name"               // 18
        /* NOTICE: Change ERRORS_SIZE macro in Messages.hpp to the current size */
    };
    
    for (int i = 0; i < ERRORS_SIZE; i++) {
        ErrorsDatabase[errorNumbers[i]] = staticErrorsString[i];
    }

}
