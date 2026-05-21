#include "TabIdGenerator.h"
TabId TabIdGenerator::create()
{
    return TabId(nextId++);
}