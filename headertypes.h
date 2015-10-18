#ifndef HTTPHEADERTYPES_H
#define HTTPHEADERTYPES_H

#include <string>

enum HeaderType
{
    // General headers
    Cache_Control,
    Connection,
    Date,
    MIME_Version,
    Pragma,
    Trailer,
    Transfer_Encoding,
    Upgrade,
    Via,
    Warning,

    //Request headers
    Accept,
    Accept_Charset,
    Accept_Encoding,
    Accept_Language,
    Authorization,
    Except,
    From,
    Host,
    If_Match,
    If_Modified_Since,
    If_None_Match,
    If_Range,
    If_Unmodified_Since,
    Max_Forwards,
    Proxy_Authorization,
    Range,
    Referer,
    TE,
    User_Agent,

    // Response headers
    Accept_Ranges,
    Age,
    Alternates,
    ETag,
    Location,
    Proxy_Authenticate,
    Public,
    Retry_After,
    Server,
    Vary,
    WWW_Authenticate,

    // Entity headers
    Allow,
    Content_Base,
    Content_Disposition,
    Content_Encoding,
    Content_Language,
    Content_Length,
    Content_Location,
    Content_MD5,
    Content_Range,
    Content_Type,
    Content_Version,
    Derived_From,
    Expires,
    Last_Modified,
    Link,
    Title,
    URI,

    Unknown
};

std::string headerToStr(HeaderType type);
HeaderType  headerToType(const std::string& str);

#endif // HTTPHEADERTYPES_H
