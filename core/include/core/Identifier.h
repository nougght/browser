#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <functional>

template <typename Tag>
struct Identifier
{
    unsigned int value;

    Identifier() : value(0) {};
    explicit Identifier(unsigned int value_) : value(value_)
    {
    }
    // operator size_t() const
    // {
    //     return value;
    // }
    
    bool equal(const Identifier &other) const
    {
        return value == other.value;
    }
    bool greater(const Identifier &other) const
    {
        return value > other.value;
    }
    bool less(const Identifier &other) const
    {
        return value < other.value;
    }

    bool operator==(const Identifier &other) const
    {
        return equal(other);
    }

    bool operator!=(const Identifier &other) const
    {
        return !equal(other);
    }

    bool operator>(const Identifier &other) const
    {
        return greater(other);
    }

    bool operator<(const Identifier &other) const
    {
        return less(other);
    }


    bool isValid() const
    {
        // std::printf("ID valid - %s\n", value != 0 ? "true" : "false");
        return value != 0;
    }
};

namespace std
{
    template <typename Tag>
struct hash<Identifier<Tag>>
    {
        size_t operator()(const Identifier<Tag> &id) const
        {
            // Просто конвертируем в size_t и используем стандартный хэш
            return std::hash<size_t>{}(static_cast<size_t>(id.value));
        }
    };
}

struct TabTag
{
};
struct HandlerTag
{
};

using TabId = Identifier<TabTag>;
using HandlerId = Identifier<HandlerTag>;

#endif
