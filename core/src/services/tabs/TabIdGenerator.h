#ifndef TAB_ID_GENERATOR_H
#define TAB_ID_GENERATOR_H
#include "core/Identifier.h"

// generator of unique auto increment tab ids
class TabIdGenerator
{
private:
    unsigned int nextId = 1;

public:
    TabId create();
    // void release(TabId id);
};
#endif
