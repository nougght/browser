#include <core/Url.h>

Url::Url(std::string value_) : value(value_)
{
}

bool Url::operator==(const Url &other) const
{
    return value == other.value;
}
