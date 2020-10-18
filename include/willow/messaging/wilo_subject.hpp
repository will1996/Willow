#pragma once
#include"willow/root/wilo_dev_core.hpp"
#include "wilo_message.hpp"
#include "willow/root/wilo_engine_element.hpp"
#include<typeindex>
#include "wilo_observer.hpp"
/*wilo_subjects are traditional subjects implemented with as much c++ 20 goodness
as possible. 

subjects work in the following, maybe convoluted way...

subject itself is a template class with one type parameter. This is meant to be the top of an inheritance hierachy
used as the message type between subjects and observers. E.G a wilo_message. 

then, permit accepts three template parameters. 
Subtype - > needs to be lower down the ineritance tree of supertyep. E.G a wilo_keyboard_message
Obs -> something in the observer higherarchy
and a method on that same observer type . 

finally, notify accepts a SUBTYPE, uses its type to look up interested observers, and delivers the SUBTYPE. 

These template parameters do all of the runtime polymorphic type checking at compile time! so these should be 
resonably fast. 

*/
//TODO implement direct messages if these end up being too slow

namespace wlo{
/*Subject internally maintains a map of triggers to observers, the template here allows the trigger, and 
   Observer reciever functions to be whatever we might need going forward, be that a message or any other message 
   like type. Also, we should be able to save on runtime type checks since observer
   - subject relationships are resolved at build time 
*/



}