#pragma once

#include <sstream>
#include <string_view>

enum class MessageLevel 
{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
};

namespace Logger 
{
    void printDebug(std::string_view message);
    void printInfo(std::string_view message);
    void printWarning(std::string_view message);
    void printError(std::string_view message);

    // High level print function which allows for variadic arguments
    template<class... Args>
    void print(MessageLevel messageLevel, const Args&... args)
    {
        std::ostringstream message;
        (message << ... << args);  // Expand arguments and send all to the message stream

        switch (messageLevel) {
            case (MessageLevel::DEBUG):
                printDebug(message.str());
                break;
            case (MessageLevel::INFO):
                printInfo(message.str());
                break;
            case (MessageLevel::WARNING):
                printWarning(message.str());
                break;
            case (MessageLevel::ERROR):
                printError(message.str());
                break;
            default:
                break;
        }
    }
}