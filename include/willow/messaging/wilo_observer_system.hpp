#include"wilo_message.hpp"
#include"wilo_observer.hpp"
#include"wilo_subject.hpp"

/*
 uses classic observers-subjects circa gang of four observers and subjecst.

Core phiosophy - 
Classes may extend subject, doing so allows classes inheriting from observer to register
to recieve messages from them. This process enables loosely coupled communication between unrelated 
sub-systems. 

*/


