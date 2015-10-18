#include "httpmessages.h"
#include <cstdlib>

ClientRequest::ClientRequest()
    :_type(None), _path()
{}

void ClientRequest::setType(RequestType type)
{
    _type = type;
}

void ClientRequest::setPath(const std::string &str)
{
    _path = str;
}

void ClientRequest::clear()
{
    _type = None;
    _path.clear();
    _version = 0;
    _headers.clear();
}

RequestType ClientRequest::type() const
{
    return _type;
}

std::string ClientRequest::path() const
{
    return _path;
}

ServerResponse::ServerResponse()
    : _statusCode(0), _statusText(), _message()
{

}

void ServerResponse::parseResponse(std::string &data)
{
    parseFirstStr(data);
    parseHeaders(data);
    getMessage(data);
}

void ServerResponse::clear()
{
    _version    = 0;
    _statusCode = 0;
    _statusText.clear();
    _headers.clear();
    _message.clear();
}

int ServerResponse::statusCode() const
{
    return _statusCode;
}

std::string ServerResponse::statusText() const
{
    return _statusText;
}

std::string ServerResponse::message() const
{
    return _message;
}

void ServerResponse::parseFirstStr(std::string& data)
{
    std::istringstream is(data);
    std::string first = getline(is);
    data.erase(data.begin(), data.begin() + is.tellg());

    is.str(first); // Changing stream for comfortable work with the string.
    char symbol;
    while (is.get(symbol) && symbol != '/')
        ; // Reading 'HTTP/'
    is >> _version;
    is >> _statusCode;
    _statusText = getline(is);
}

void ServerResponse::parseHeaders(std::string& data)
{
    std::istringstream  is(data);
    std::istringstream  local;
    std::string         currStr;
    std::string         header;
    std::string         value;

    while (1) // Exiting only when all headers will parsed.
    {
        currStr = getline(is);
        if (currStr.empty()) // All headers were parsed.
            break;

        local.clear();
        local.str(currStr); // Changing stream for comfortable work with string.
        header = getline(local, ':', ' ');
        value = getline(local);
        correctHeader(header);

        setHeaderValue(headerToType(header), value);
    }

    data.erase(0, is.tellg());
}

void ServerResponse::getMessage(std::string& data)
{
    _message = data;
    data.clear();
}

void ServerResponse::correctHeader(std::string& str)
{
    int size = str.size();
    for (int i = 0; i < size; ++i)
    {
        if (i == 0)
        {
            if (islower(str[i]))
                str[i] = toupper(str[i]);
        }
        else if (str[i] == '-')
            continue;
        else if (str[i-1] == '-')
        {
            if (islower(str[i]))
                str[i] = toupper(str[i]);
        }
        else if (isupper(str[i]))
            str[i] = tolower(str[i]);
    }
}

std::string ServerResponse::getline(std::istringstream& is, char delim1, char delim2) const
{
    std::string str;
    char symbol;
    while (is.get(symbol) && !is.eof() && symbol != delim1 && symbol != delim2)
        str += symbol;

    if (symbol == delim1 && !is.eof())
    {
        is.get(symbol);
        if (symbol != delim2)
            is.putback(symbol);
    }
    return str;
}
