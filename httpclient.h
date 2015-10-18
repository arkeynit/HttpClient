#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "httpmessages.h"
#include "clientactions.h"
#include "requesttypes.h"

#include <arpa/inet.h>

class HttpClient
{

public:
    HttpClient();
    ~HttpClient();

    void         tryDownload(std::string &address, const std::string &dirPath);

private:
    bool         parseAddress(std::string &str);

    bool         createSocket();
    bool         connect(const std::string &address);
    std::string  createRequest() const;
    bool         sendRequest() const;
    bool         receive(std::string &data, int size = 2048);

    void         download();
    void         save();

    ClientAction selectAction();
    void         doAction(ClientAction act);

    void         notifyUser(const std::string &mess) const;

    ClientRequest   _request;
    ServerResponse  _response;

    int             _socket;
    sockaddr_in     _serverInfo;

    std::string     _address; // Server address.
    std::string     _path;    // Path to resourse.
    std::string     _dirPath; // For downloading.
};

#endif // HTTPCLIENT_H
