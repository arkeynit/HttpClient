#ifndef SERVRESPONSE_H
#define SERVRESPONSE_H

#include <string>
#include <map>
#include <sstream>

#include "headertypes.h"
#include "requesttypes.h"

class HttpMessages
{
public:
    void setHeaderValue(HeaderType type, const std::string &value)
    {
        std::map<HeaderType, std::string>::iterator it;
        it = _headers.find(type);
        if (it == _headers.end())
            _headers.insert(std::make_pair(type, value));
        else
            it->second = value; // Replacing
    }

    void setHeaders(const std::map<HeaderType, std::string> &headers)
    {
        _headers = headers;
    }

    void setVersion(double version)
    {
        _version = version;
    }

    virtual void clear()
    {
        _version = 0;
        _headers.clear();
    }

    std::string headerValue(HeaderType type) const
    {
        std::map<HeaderType, std::string>::const_iterator it;
        it = _headers.find(type);
        if (it != _headers.end())
            return it->second;
        else
            return "";
    }

    const std::map<HeaderType, std::string> headers() const
    {
        return _headers;
    }

    double version() const
    {
        return _version;
    }

protected:
    HttpMessages()
        : _version(0), _headers()
    {}

    double _version;
    std::map<HeaderType, std::string> _headers;
};

class ClientRequest: public HttpMessages
{
public:
    ClientRequest();

    void        setType(RequestType type);
    void        setPath(const std::string &path);
    void        clear();

    RequestType type() const;
    std::string path() const;
private:
    RequestType _type;
    std::string _path;
};

class ServerResponse: public HttpMessages
{
public:
    ServerResponse();

    void        parseResponse(std::string &data);
    void        clear();

    int         statusCode() const;
    std::string statusText() const;
    std::string message()    const;
private:
    void        parseFirstStr(std::string &data);
    void        parseHeaders(std::string &data);
    void        getMessage(std::string &data);

    void        correctHeader(std::string &str);
    std::string getline(std::istringstream &is, char delim1 = '\r', char delim2 = '\n') const;

    int         _statusCode;
    std::string _statusText;
    std::string _message;
};

#endif // SERVRESPONSE_H
