#ifndef WILLOW_MESSAGE_SYSTEM_H
#define WILLOW_MESSAGE_SYSTEM_H
#include"Messages.hpp"
#include"wilo_observer.hpp"
#include"wilo_subject.hpp"

/*
 uses classic observers-subjects circa gang of four observers and subjecst.

Core phiosophy - 
Classes may extend subject, doing so allows classes inheriting from observer to register
to recieve messages from them. This process enables loosely coupled communication between unrelated 
sub-systems. 

*/


namespace wlo{

   class MessageSystem {
   public:
       class Subject;
       class Observer  : public EngineElement{
       public:
           Observer() = default;

           void reclaim();
           virtual ~Observer(){reclaim();}

           ID_type  getID(){
               return m_ID;
           }
           //we need this list of subjects to notify when this obererver is freed, this is how we populate that list.
           template<typename SUBTYPE>
           void trackSubject(Subject* sub);
       private:
           std::vector<Subject*> m_subjects;
           std::vector<std::function<void(Subject*)> > m_revokerFunctions;
       };

       class Subject : public EngineElement {
       public :
           Subject() = default;
       //register observers on this subject
           template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const SUBTYPE &)>
           void permit(Observer *obs);

           template<typename SUBTYPE>
           void notify(const SUBTYPE &t);

           template<typename SUBTYPE>
           void revoke(ID_type id);

       private:
           template<typename SUBTYPE>
           using invoker_t = void (*)(MessageSystem::Observer * obs,const SUBTYPE &);

           template<typename SUBTYPE>
           struct Invocation {
               invoker_t<SUBTYPE> inv;
               Observer *obs;

               void invoke(const SUBTYPE &msg) {
                   inv(obs, msg);
               }

               Invocation(Observer *a_obs, invoker_t<SUBTYPE> a_inv) : obs(a_obs), inv(a_inv) {}

               Invocation() = default;
           };

           template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const SUBTYPE &subtype)>
           static void invoker(MessageSystem::Observer *obs, const SUBTYPE &msg) {
               (static_cast<Obs *>(obs)->*Method)(static_cast<const SUBTYPE &>(msg));
           }

           template<typename SUBTYPE>
           static std::map<ID_type, Invocation<SUBTYPE> > m_registry;
           std::map<std::type_index, std::vector<ID_type> > m_triggerFilter;
       };
   };
}

//Definitions
namespace wlo{


    inline void MessageSystem::Observer::reclaim() {
        for(auto * sub : m_subjects) {
            for(auto && revoker : m_revokerFunctions){
                revoker(sub);
            }
        }

    }

    template<class SUBTYPE>
    void MessageSystem::Observer::trackSubject(Subject *sub) {
        m_subjects.push_back(sub);
        m_revokerFunctions.push_back(
                [&](Subject * sub){
                    sub->revoke<SUBTYPE>(m_ID);
                }
        );
    }



    template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const SUBTYPE &)>
    //register method for objects
    //TODO enforce Observer interface with concept
    void MessageSystem::Subject::permit(Observer *obs) {
        //get the observer's unique ID to track it
        ID_type uniqueID = obs->getID();
        //add the "this" pointer to our observer so it can notify this subject upon its desconstruction
        obs->trackSubject<SUBTYPE>(this);
        m_triggerFilter[typeid(SUBTYPE)].push_back(uniqueID);//add this observer

        WILO_CORE_INFO("permitting observer with ID {0} triggering on  {1}'s total registered observers:{2} \n",
                       uniqueID, typeid(SUBTYPE).name(), m_triggerFilter.size());

        invoker_t <SUBTYPE> inver = &invoker<SUBTYPE, Obs, Method>;
        Invocation inv(obs, inver);
        m_registry<SUBTYPE>[uniqueID] = inv;
    };

    template<typename SUBTYPE>
    void MessageSystem::Subject::notify(const SUBTYPE &t) {
        //get the list of uniqueID's for the trigger filter
        if (!m_triggerFilter.count(typeid(SUBTYPE))) {
            WILO_CORE_ERROR("ATTEMPT TO NOTIFY ON UNBOUND MESSAGE TYPE");
            //throw std::runtime_error("attempt to notify an unbound message type, failing");
        }
        for (ID_type id : m_triggerFilter[typeid(SUBTYPE)]) {
            m_registry<SUBTYPE>[id].invoke(t);//call the bound method for each ID
        };
    };


    template<typename SUBTYPE>
    void MessageSystem::Subject::revoke(ID_type id) {
        m_registry < SUBTYPE >.erase(id);//remove all bound methods for the ID
    };

    template<typename SUBTYPE>//allocate storage
    std::map<ID_type, MessageSystem::Subject::Invocation<SUBTYPE> >  MessageSystem::Subject::m_registry;

}





#endif
