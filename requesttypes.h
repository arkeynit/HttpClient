#ifndef REQUESTTYPES_H
#define REQUESTTYPES_H

#include <string>

enum RequestType
{
    None,

    HEAD,
    GET
};

const std::string requestTypeToStr(RequestType type);

#endif // REQUESTTYPES_H
