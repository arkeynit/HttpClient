#include "headertypes.h"

HeaderType headerToType(const std::string& str)
{
    if (str == "Accept")
        return Accept;
    else if (str == "Accept-Charset")
        return Accept_Charset;
    else if (str == "Accept-Encoding")
        return Accept_Encoding;
    else if (str == "Accept-Language")
        return Accept_Language;
    else if (str == "Accept-Ranges")
        return Accept_Ranges;
    else if (str == "Age")
        return Age;
    else if (str == "Allow")
        return Allow;
    else if (str == "Alternates")
        return Alternates;
    else if (str == "Authorization")
        return Authorization;
    else if (str == "Cache-Control")
        return Cache_Control;
    else if (str == "Connection")
        return Connection;
    else if (str == "Content-Base")
        return Content_Base;
    else if (str == "Content-Disposition")
        return Content_Disposition;
    else if (str == "Content-Encoding")
        return Content_Encoding;
    else if (str == "Content-Language")
        return Content_Language;
    else if (str == "Content-Length")
        return Content_Length;
    else if (str == "Content-Location")
        return Content_Location;
    else if (str == "Content-MD5")
        return Content_MD5;
    else if (str == "Content-Range")
        return Content_Range;
    else if (str == "Content-Type")
        return Content_Type;
    else if (str == "Content-Version")
        return Content_Version;
    else if (str == "Date")
        return Date;
    else if (str == "Derived-From")
        return Derived_From;
    else if (str == "ETag")
        return ETag;
    else if (str == "Except")
        return Except;
    else if (str == "Expires")
        return Expires;
    else if (str == "From")
        return From;
    else if (str == "Host")
        return Host;
    else if (str == "If-Match")
        return If_Match;
    else if (str == "If-Modified-Since")
        return If_Modified_Since;
    else if (str == "If-None-Match")
        return If_None_Match;
    else if (str == "If-Range")
        return If_Range;
    else if (str == "If-Unmodified-Since")
        return If_Unmodified_Since;
    else if (str == "Last-Modified")
        return Last_Modified;
    else if (str == "Link")
        return Link;
    else if (str == "Location")
        return Location;
    else if (str == "Max-Forwards")
        return Max_Forwards;
    else if (str == "MIME-Version")
        return MIME_Version;
    else if (str == "Pragma")
        return Pragma;
    else if (str == "Proxy-Authenticate")
        return Proxy_Authenticate;
    else if (str == "Proxy-Authorization")
        return Proxy_Authorization;
    else if (str == "Public")
        return Public;
    else if (str == "Range")
        return Range;
    else if (str == "Referer")
        return Referer;
    else if (str == "Retry-After")
        return Retry_After;
    else if (str == "Server")
        return Server;
    else if (str == "Title")
        return Title;
    else if (str == "TE")
        return TE;
    else if (str == "Trailer")
        return Trailer;
    else if (str == "Trailer-Encoding")
        return Transfer_Encoding;
    else if (str == "Upgrade")
        return Upgrade;
    else if (str == "URI")
        return URI;
    else if (str == "User-Agent")
        return User_Agent;
    else if (str == "Vary")
        return Vary;
    else if (str == "Via")
        return Via;
    else if (str == "Warning")
        return Warning;
    else if (str == "WWW-Authenticate")
        return WWW_Authenticate;
    else
        return Unknown;
}

std::string headerToStr(HeaderType type)
{
    switch(type)
    {
    case Cache_Control:
        return "Cache-Control";
    case Connection:
        return "Connection";
    case Date:
        return "Date";
    case MIME_Version:
        return "MIME-Version";
    case Pragma:
        return "Pragma";
    case Trailer:
        return "Trailer";
    case Transfer_Encoding:
        return "Transfer-Encoding";
    case Upgrade:
        return "Upgrade";
    case Via:
        return "Via";
    case Warning:
        return "Warning";
    case Accept:
        return "Accept";
    case Accept_Charset:
        return "Accept-Charset";
    case Accept_Encoding:
        return "Accept-Encoding";
    case Accept_Language:
        return "Accept-Language";
    case Authorization:
        return "Authorization";
    case Except:
        return "Except";
    case From:
        return "From";
    case Host:
        return "Host";
    case If_Match:
        return "If-Match";
    case If_Modified_Since:
        return "If-Modified-Since";
    case If_None_Match:
        return "If-None-Match";
    case If_Range:
        return "If-Range";
    case If_Unmodified_Since:
        return "If-Unmodified-Since";
    case Max_Forwards:
        return "Max-Forwards";
    case Proxy_Authorization:
        return "Proxy-Authorization";
    case Range:
        return "Range";
    case Referer:
        return "Referer";
    case TE:
        return "TE";
    case User_Agent:
        return "User-Agent";
    case Accept_Ranges:
        return "Accept-Ranges";
    case Age:
        return "Age";
    case Alternates:
        return "Alternates";
    case ETag:
        return "ETag";
    case Location:
        return "Location";
    case Proxy_Authenticate:
        return "Proxy-Authenticate";
    case Public:
        return "Public";
    case Retry_After:
        return "Retry-After";
    case Server:
        return "Server";
    case Vary:
        return "Vary";
    case WWW_Authenticate:
        return "WWW-Authenticate";
    case Allow:
        return "Allow";
    case Content_Base:
        return "Content-Base";
    case Content_Disposition:
        return "Content-Disposition";
    case Content_Encoding:
        return "Content-Encoding";
    case Content_Language:
        return "Content-Language";
    case Content_Length:
        return "Content-Length";
    case Content_Location:
        return "Content-Location";
    case Content_MD5:
        return "Content-MD5";
    case Content_Range:
        return "Content-Rage";
    case Content_Type:
        return "Content-Type";
    case Content_Version:
        return "Content-Version";
    case Derived_From:
        return "Derived-From";
    case Expires:
        return "Expires";
    case Last_Modified:
        return "Last-Modified";
    case Link:
        return "Link";
    case Title:
        return "Title";
    case URI:
        return "URI";
    default:
        return "Unknown";
    }
}
