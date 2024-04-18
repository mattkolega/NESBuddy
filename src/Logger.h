#pragma once

#include <string_view>

namespace Logger 
{
    void printDebug(std::string_view message);
    void printInfo(std::string_view message);
    void printWarning(std::string_view message);
    void printError(std::string_view message);
}