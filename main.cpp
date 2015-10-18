#include "httpclient.h"

#include <iostream>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cout << "Address didn't input." << std::endl;
        return 1;
    }

    HttpClient client;
    std::string address = std::string(argv[1], strlen(argv[1]));
    std::string dirPath = ".";

    client.tryDownload(address, dirPath);

    return 0;
}
