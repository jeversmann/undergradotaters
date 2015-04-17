
#include "c_breeze.h"
#include "pointervalue.h"

// ------------------------------------------------------------
// pointerValue class
// ------------------------------------------------------------

pointerValue::pointerValue()
  : blocks(),
    // value(0),
    constant_value(0),
    is_address(false),
    is_a_use(false),
    dereferenced()
{}

pointerValue::pointerValue(memoryBlock * mb, bool is_address)
  : blocks(),
    // value(0),
    constant_value(0),
    is_address(is_address),
    is_a_use(false),
    dereferenced()
{
  blocks.insert(mb);
}

