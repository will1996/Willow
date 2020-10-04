#pragma once
#include"willow/root/wilo_dev_core.hpp"
#include "wilo_message.hpp"
#include "willow/root/wilo_engine_element.hpp"
#include<typeindex>
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

//TODO replace current implementation with Xylem inspired storage with option for direct messages


namespace wlo{
/*Subject internally maintains a map of triggers to observers, the template here allows the trigger, and 
   Observer reciever functions to be whatever we might need going forward, be that a message or any other message 
   like type. Also, we should be able to save on runtime type checks since observer
   - subject relationships are resolved at build time 
*/

class Observer;


class Subject: public EngineElement{
        typedef void(*invoker_t)(Observer *,const Message& msg);
    struct Invocation {
        invoker_t inv;
        Observer* obs;
        void invoke(const Message& msg){
            inv(obs,msg);
        }
       Invocation(Observer* a_obs,invoker_t a_inv) : obs(a_obs),inv(a_inv) {}
       Invocation() = default;
    };

    public :
        Subject();
        Subject(ID_type);
        virtual void initialize() override;
        virtual void reclaim() override;


        template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const Message&)>//register method for objects
        //TODO enforce Observer interface with concept 
        void permit(Obs* obs){
                //get the observer's unique ID to track it
               ID_type uniqueID = obs->getID();
               //add the "this" pointer to our observer so it can notify this subject upon its desconstruction
               obs->trackSubject(this);
               m_triggerFilter[typeid(SUBTYPE)].push_back(uniqueID);//add this observer 
                 
               WILO_CORE_INFO("permitting observer with ID {0} triggering on  {1}'s total registered observers:{2} \n",uniqueID, typeid(SUBTYPE).name(),m_triggerFilter.size());
                
               invoker_t inver = &invoker<SUBTYPE,Obs,Method>;
               Invocation inv(obs,inver);
               m_registry[uniqueID][typeid(SUBTYPE)] = inv; 

        };
        template<typename SUBTYPE>
        void notify(const SUBTYPE& t){
                //get the list of uniqueID's for the trigger filter
            if (! m_triggerFilter.count(typeid(SUBTYPE))){
                WILO_CORE_ERROR("ATTEMPT TO NOTIFY ON UNBOUND MESSAGE TYPE");
                //throw std::runtime_error("attempt to notify an unbound message type, failing");
            }
            for (ID_type id : m_triggerFilter[typeid(SUBTYPE)]){
                m_registry[id][typeid(SUBTYPE)].invoke(t);//call the bound method for each ID
            };
        };

        void revoke(ID_type id){
                    m_registry.erase(id);//remove all bound methods for the ID 
        };
    private:
            template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const Message& subtype)>
        static void invoker( Observer* obs, const Message& msg){
                (static_cast<Obs*>(obs)->*Method)(static_cast<const SUBTYPE&>(msg));
        }
        std::map<ID_type, std::map<std::type_index,Invocation> >  m_registry;
        std::map<std::type_index,std::vector<ID_type> > m_triggerFilter;
};

}