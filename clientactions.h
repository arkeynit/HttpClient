#ifndef CLIENTACTIONS_H
#define CLIENTACTIONS_H

enum ClientAction
{
    DoNothing,
    Download,
    Save,
    RequestFromOtherLocation,
    OutputRequest,
    NotifyAboutLogicError,
    RepeatRequest,
    RepeatWithOtherHTTPVersion
};

#endif // CLIENTACTIONS_H
