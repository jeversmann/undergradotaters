
#include "c_breeze.h"
#include "pointervalue.h"

// ------------------------------------------------------------
// pointerValue class
// ------------------------------------------------------------

int pointerValue::count = 0;

pointerValue::pointerValue()
  : blocks(),
    // value(0),
    constant_value(0),
    is_address(false),
    is_a_use(false),
    dereferenced()
{
  count++;
}

pointerValue::pointerValue(memoryBlock * mb, bool is_address)
  : blocks(),
    // value(0),
    constant_value(0),
    is_address(is_address),
    is_a_use(false),
    dereferenced()
{
  blocks.insert(mb);
  count++;
}
/** @brief Copy constructor
 */

pointerValue::pointerValue(const pointerValue & other)
  : blocks(other.blocks),
    constant_value(other.constant_value),
    is_address(other.is_address),
    is_a_use(other.is_a_use),
    dereferenced(other.dereferenced)
{
  count++;
}

 
pointerValue::~pointerValue()
{
  count--;
}
