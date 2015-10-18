#include "httpclient.h"

#include <netdb.h>
#include <sys/socket.h>

#include <unistd.h>
#include <iostream>
#include <locale>
#include <fstream>

using namespace std;

// Function for convert of values to string.
// Warning! Use only for types, which has definition for operator << .
template<class T>
string convertToStr(const T& t)
{
    stringstream ss;
    ss << t;
    return ss.str();
}

HttpClient::HttpClient()
    :  _request(), _response(), _socket(-1), _address(), _dirPath()
{
    _serverInfo.sin_family      = 0;
    _serverInfo.sin_port        = 0;
    _serverInfo.sin_addr.s_addr = 0;
}

HttpClient::~HttpClient()
{
    if (_socket)
        close(_socket);
}

void HttpClient::tryDownload(string &address, const string &dirPath)
{
    parseAddress(address);

    cout << "address: " << _address << endl;
    cout << "path: " << _path << endl;
    _dirPath = dirPath;

    if (!createSocket())
        return;

    if (!connect(_address))
        return;

    notifyUser("Prepare for requesting...");
    _request.setType(HEAD);
    _request.setPath(_path);
    _request.setVersion(1.1);
    _request.setHeaderValue(Host, _address);

    if (!sendRequest())
        return;

    string buffer;
    if (!receive(buffer))
        return;

    _response.parseResponse(buffer);

    doAction(selectAction());
}

bool HttpClient::parseAddress(std::string &str)
{
    _address.clear();
    _path.clear();
    int size = str.size();

    // Trying to find '://'
    int addressPos = -1;
    for (int i = 0; i < size - 2; ++i)
    {
        if (str[i] == ':' && str[i + 1] == '/' && str[i + 2] == '/')
        {
            addressPos = i + 3;
            break;
        }
    }

    if (addressPos == -1) // Didn't find
        addressPos = 0;

    // Trying to find end of address
    for (int i = addressPos; i < size; ++i)
    {
        if (str[i] != '/')
            continue;

        // Found
        for (int j = addressPos; j < i; ++j)
            _address += str[j];

        if (_address.empty())
            return false;

        if (i == size - 1)
        {
            _path = "/";
            return true;
        }

        for (int j = i; j < size; ++j)
            _path += str[j];
        return true;
    }

    _address = str.substr(addressPos, size - addressPos);
    _path = "/";
    return true;
}

bool HttpClient::createSocket()
{
    if (_socket != -1)
    {
        notifyUser("Socket was created.");
        return true;
    }

    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == -1)
    {
        notifyUser("Socket has not been created.");
        return false;
    }
    else
    {
        notifyUser("Socket was created.");
        return true;
    }
}

bool HttpClient::connect(const string &address)
{
    const int HTTP_Port = 80;

    _serverInfo.sin_family      = AF_INET;
    _serverInfo.sin_port        = htons(HTTP_Port);
    _serverInfo.sin_addr.s_addr = inet_addr(address.c_str());

    notifyUser("Connecting...");

    if (_serverInfo.sin_addr.s_addr != -1)
    {
        if (::connect(_socket, (const sockaddr*)(&_serverInfo), sizeof(_serverInfo)) < 0)
        {
            notifyUser("Connect failed.");
            return false;
        }
        else
        {
            notifyUser("Connected.");
            return true;
        }
    }

    hostent *hostInfo = gethostbyname(address.c_str());
    if (!hostInfo)
    {
        notifyUser("Incorrect address name or Internet connection does not exist.");
        return false;
    }

    in_addr **addressList = (in_addr**) hostInfo->h_addr_list;

    for (int i = 0; addressList[i] != NULL; ++i)
    {
        _serverInfo.sin_addr = *addressList[i];
        if (_serverInfo.sin_addr.s_addr == -1)
            continue;

        if (::connect(_socket, (const sockaddr*)(&_serverInfo), sizeof(_serverInfo)) == 0) // Sucessfully
        {
            notifyUser("Connected.");
            return true;
        }
    }

    notifyUser("Connect failed.");
    return false;
}

string HttpClient::createRequest() const
{
    const string strEnd = "\r\n";

    string data;
    data += requestTypeToStr(_request.type()) + ' ';
    data += _request.path() + ' ';
    data += "HTTP/" + convertToStr(_request.version()) + strEnd;

    const map<HeaderType, string> &headers = _request.headers();
    for (map<HeaderType, string>::const_iterator it = headers.begin();
         it != headers.end();
         ++it)
    {
        data += headerToStr(it->first) + ": " + it->second + strEnd;
    }
    data += strEnd;

    return data;
}

bool HttpClient::sendRequest() const
{
    string data = createRequest();

    if (send(_socket, data.c_str(), data.size(), 0) < 0)
    {
        notifyUser("Must not send request.");
        return false;
    }
    else
    {
        notifyUser("Request sent.");
        return true;
    }
}

bool HttpClient::receive(string& data, int size)
{
    data.clear();
    data.resize(size);
    notifyUser("Receiving...");
    int returnValue = recv(_socket, &data[0], size, 0);
    if (returnValue < 0)
    {
        notifyUser("Client could not receive a request from the server.");
        data.clear();
        return false;
    }
    else if (returnValue == 0)
    {
        data.clear();
        return false;
    }
    else
    {
        data.resize(returnValue);
        notifyUser("Response was received.");
        return true;
    }
}

void HttpClient::download()
{
    notifyUser("Prepare for downloading...");
    _request.setType(GET);
    _request.setHeaderValue(Connection, "Close");
    if (!sendRequest())
        return;

    _response.clear();
    string buf;
    receive(buf);
    _response.parseResponse(buf);

    doAction(selectAction());
}

void HttpClient::save()
{
    std::string messBuf = _response.message();
    if (messBuf.empty())
    {
        notifyUser("Message from server is empty.");
        return;
    }
    else
        notifyUser("Prepare for saving...");

    std::string fileName = _address + ".html";
    std::string path = _dirPath + "/" + fileName;
    ofstream file;
    file.open(path.c_str());
    while (!file.is_open())
    {
        notifyUser("Must not to create file in the directory: " + path);
        notifyUser("Enter new directory path please:");
        cin >> path;
        path += "/" + fileName;
        file.open(path.c_str());
    }

    string sizeValue = _response.headerValue(Content_Length);
    int fileSize = 0;

    file << messBuf;
    if (!sizeValue.empty())
    {
        fileSize += messBuf.size();
        notifyUser("Saved " + convertToStr(fileSize) + " from " + sizeValue + " bytes.");
    }

    do
    {
        if (!receive(messBuf))
            break;
        file << messBuf;

        if (!sizeValue.empty())
        {
            fileSize += messBuf.size();
            notifyUser("Saved " + convertToStr(fileSize) + " from " + sizeValue + " bytes.");
        }

    } while (!messBuf.empty());
    notifyUser("File with name \"" + fileName + "\" saved successfully.");
}

void HttpClient::notifyUser(const string& mess) const
{
    cout << mess << endl;
}

ClientAction HttpClient::selectAction()
{
    int status = _response.statusCode();
    switch(status)
    {
    case 105: // Name Not Resolved
        notifyUser("Incorrect addess or does not exist.");
        return DoNothing;
    case 100: // Continue
    case 101: // Switching Protocols
    case 200: // OK
    case 203: // Non-Authoritative Information
    case 204: // No Content
        if (_request.type() == HEAD)
            return Download;
        else if (_request.type() == GET)
            return Save;
        else
            return NotifyAboutLogicError;
    case 301: // Moved Permanently
    case 302: // Moved Temporarily / Found
    case 303: // See Other
    case 307: // Temporary Redirect
        if (_request.type() != GET)
        {
            notifyUser("Server redirected on other location.");
            return RequestFromOtherLocation;
        }
        else
        {
            notifyUser("Server again change location.");
            return Save;
        }
    case 400: // Bad Request
        return OutputRequest;
    case 401: // Unauthorized
        notifyUser("Identification is required to access the requested resource.");
        return DoNothing;
    case 403: // Forbidden
        notifyUser("Resourse has restictions, therefore must not get it.");
        return DoNothing;
    case 404: // Not Found
        notifyUser("Resourse not found.");
        return DoNothing;
    case 405: // Method Not Allowed
        notifyUser("Server not allowed method: "
                    + requestTypeToStr(_request.type()) + ".");
        return DoNothing;
    case 407: // Proxy Authentication Required
        notifyUser("Identification is required to access the requested resource, "
                       "because you are using proxy.");
        return DoNothing;
    case 408: // Request Timeout
        return RepeatRequest;
    case 410: // Gone
        notifyUser("Resourse was deleted and is no longer available.");
        return DoNothing;
    case 414: // Request-URL Too Long
        notifyUser("You input address with too long path to resourse.");
        return DoNothing;
    case 415: // Unsupported Media Type
        notifyUser("The server is refusing to process a request for an unknown reason.");
        return DoNothing;
    case 418: // I'm a teapot
        notifyUser("The server can joke!!!");
        return DoNothing;
    case 434: // Requested host unavailable
        notifyUser("The requested server is not available.");
        return DoNothing;
    case 451: // Unavailable For Legal Reasons
        notifyUser("Access to the resource is closed for legal reasons.");
        return DoNothing;
    case 500: // Internal Server Error
        notifyUser("Internal server error.");
        return DoNothing;
    case 502: // Bad Gateway
        notifyUser("Bad Gateway.");
        return DoNothing;
    case 503: // Service Unavailable
        notifyUser("Service unavailable.");
        return DoNothing;
    case 504: // Gateway Timeout
        notifyUser("Gateway timeout.");
        return DoNothing;
    case 505: // HTTP Version Not Supported
        notifyUser("HTTP version not supported.");
        return RepeatWithOtherHTTPVersion;
    case 102: // Processing
    case 201: // Created
    case 202: // Accepted
    case 205: // Reset Content
    case 206: // Partial Content
    case 207: // Multi-Status
    case 226: // IM Used
    case 300: // Multiple Choises
    case 304: // Non Modified
    case 305: // Use Proxy
    case 306: // Reserved
    case 402: // Payment Required
    case 406: // Not Acceptable
    case 409: // Conflict
    case 411: // Length Required
    case 412: // Precondition Failed
    case 413: // Request Entity Too Large
    case 416: // Requested Range Not Satisfiable
    case 417: // Expectation Failed
    case 422: // Unprocessable Entity
    case 423: // Locked
    case 424: // Failed Dependency
    case 425: // Unordered Collection
    case 426: // Upgrade Required
    case 428: // Precondition Required
    case 429: // Too Many Requests
    case 431: // Request Header Fields Too Large
    case 449: // Retry With
    case 456: // Unrecoverable Error
    case 501: // Not Implemented
    case 506: // Variant Also Negotiates
    case 507: // Insufficient Storage
    case 509: // Bandwidth Limit Exceeded
    case 510: // Not Extended
    case 511: // Network Authentication Required
    default:
        notifyUser("The client does not know how to handle the response with code "
                   + convertToStr(status) + ".\n"
                   "Description code: " + _response.statusText() + ".");
        return DoNothing;
    }

    return NotifyAboutLogicError; // Oops, I missed somewhere
}

void HttpClient::doAction(ClientAction act)
{
    switch(act)
    {
    case Download:
        download();
        return;
    case Save:
        save();
        return;
    case RequestFromOtherLocation:
    {
        HeaderType header = Location;
        std::string value = _response.headerValue(header);
        if (value.empty())
        {
            notifyUser("Server doesn\'t put back the header: " + headerToStr(header));
            return;
        }
        parseAddress(value);
        _request.setPath(value);
        _request.setHeaderValue(Host, _address);
        notifyUser("Redirecting...");
        download();
        return;
    }
    case OutputRequest:
        notifyUser("Server found syntax error in request.");
        notifyUser("Request:");
        cout << createRequest() << endl;
        return;
    case NotifyAboutLogicError:
        notifyUser("Oops! A logical mistake is occured!");
        return;
    case RepeatRequest:
    {
        if (!sendRequest())
            return;
        string buf;
        if (!receive(buf))
            return;
        _response.clear();
        _response.parseResponse(buf);
        ClientAction newAct = selectAction();
        if (act == newAct)
        {
            notifyUser("Server didn\'t understand request.");
            return;
        }
        doAction(newAct);
        return;
    }
    case RepeatWithOtherHTTPVersion:
    {
        _request.setVersion(1.0);
        sendRequest();
        string buf;
        if (!receive(buf))
            return;
        _response.clear();
        _response.parseResponse(buf);
        doAction(selectAction());
        return;
    }
    case DoNothing:
    default:
        return;
    }
}
