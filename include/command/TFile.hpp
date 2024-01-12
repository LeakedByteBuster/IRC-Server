#pragma once
#include <iostream>
#include <sstream>
#include <fstream>

// file has four member sender which the name of client that send the file
// and reciever the client that recive the file
// and the filename and stream of file

class TFile
{
    private:
    std :: string const sender;
    std :: string const receiver;
    std :: string const filename;
    std::FILE *  fd;

    public :

    // constructor
    TFile(std::FILE *Fd,std :: string const Filename,std :: string const Sender,std :: string Receiver) :sender(Sender),receiver(Receiver),filename(Filename),fd(Fd)
    {
    };

    //getters
    std :: string getFilname(){return filename;};
    std :: string getReceiver(){return receiver;};
    std :: string getSender(){return sender;};
    std::FILE*  getstream(){return fd;};
    ~TFile(){};
};