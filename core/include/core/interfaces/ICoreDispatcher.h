#ifndef I_CORE_DISPATCHER_H
#define I_CORE_DISPATCHER_H

#include <functional>

class ICoreDispatcher
{
    public:
    virtual void post(std::function<void()> task)  = 0;
};

#endif
