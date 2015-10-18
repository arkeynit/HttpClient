#include "requesttypes.h"

const std::string requestTypeToStr(RequestType type)
{
    switch(type)
    {
    case HEAD:
        return "HEAD";
    case GET:
        return "GET";
    default:
        return "";
    }
    return "";
}
