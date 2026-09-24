#pragma once

#include <stdexcept>
#include <string>

namespace offload
{

class OffloadException : public std::runtime_error
{
public:
    explicit OffloadException(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

}
