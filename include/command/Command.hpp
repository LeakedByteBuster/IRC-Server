#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "TFile.hpp"
#include "Messages.hpp"
#include "channel.hpp"
#include <utility>

#define MSG_MAX 512


class Server;

enum    Commands {
    UNKNOWN,
    SENDFILE,
    GETFILE,
    NICK,
    PASS_USER,
    PRVMSG,
    PONG,
    IRCBOT,
    DATE,
    JOKE,
    WHOAMI,
    JOIN
};




