#if !defined(TFILE_HPP)
#define TFILE_HPP

#include <iostream>
#include <sstream>
#include <fstream>

// file has four member sender which the name of client that send the file
// and reciever the client that recive the file
// and the filename and stream of file

class TFile
{
    private:
    std :: string  sender;
    std :: string  receiver;
    std :: string  filename;
    std::FILE *  fd;

    public :

    // Constructor
    TFile(std::FILE *Fd,std :: string  Filename,std :: string  Sender,std :: string Receiver) :sender(Sender),receiver(Receiver),filename(Filename),fd(Fd)
    {
    };

    //getters
    std :: string getFilname(){return filename;};
    std :: string getReceiver(){return receiver;};
    std :: string getSender(){return sender;};
    std::FILE*  getstream(){return fd;};
    ~TFile(){};
};

#endif // TFILE_HPP