#ifndef WILLOW_MESSAGE_SYSTEM_H
#define WILLOW_MESSAGE_SYSTEM_H
#include"willow/messaging/Messages.hpp"
#include"willow/root/Tag.hpp"
#include<typeindex>
#include<functional>
/*
 MessageSystem:
 Enables generic, high speed messaging between unrelated components.
 uses classic observers-subjects circa gang of four observers and subjecst.

 TODO:
 Fold in some sort of direct message system if it would be faster
 Create an async queue mode

*/


namespace wlo{

   class MessageSystem {
   public:
       class Subject;
       class Observer  : Tag{
       public:
           Observer() = default;

           void reclaim();
           virtual ~Observer(){reclaim();}

           ID_type  getID(){
               return id;
           }
           //we need this list of subjects to notify when this obererver is freed, this is how we populate that list.
           template<typename SUBTYPE>
           void trackSubject(Subject* sub);
       private:
           std::vector<Subject*> m_subjects;
           std::vector<std::function<void(Subject*)> > m_revokerFunctions;
       };

       class Subject :Tag{
       public :
           Subject() = default;
           Subject(wlo::ID_type _id):Tag(_id){};
       //register observers on this subject
           template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const SUBTYPE &)>
           void permit(Observer *obs);

           template<typename SUBTYPE>
           void notify(const SUBTYPE &t)const ;

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

               Invocation(Observer *a_obs, invoker_t<SUBTYPE> a_inv) : inv(a_inv), obs(a_obs) {}

               Invocation() = default;
           };

           template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const SUBTYPE &subtype)>
           static void invoker(MessageSystem::Observer *obs, const SUBTYPE &msg) {
               (static_cast<Obs *>(obs)->*Method)(static_cast<const SUBTYPE &>(msg));
           }

           template<typename SUBTYPE>
           static std::map<ID_type, Invocation<SUBTYPE> > m_registry;
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
                    sub->revoke<SUBTYPE>(id);
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
//        m_triggerFilter[typeid(SUBTYPE)].push_back(uniqueID);//add this observer


        invoker_t <SUBTYPE> inver = &invoker<SUBTYPE, Obs, Method>;
        Invocation inv(obs, inver);
        m_registry<SUBTYPE>[uniqueID] = inv;
    };

    template<typename SUBTYPE>
    void MessageSystem::Subject::notify(const SUBTYPE &t) const {
        //get the list of uniqueID's for the trigger filter
        for(auto &[_id,invoker]:m_registry<SUBTYPE>)
            if(_id!=id)
            invoker.invoke(t);
    };


    template<typename SUBTYPE>
    void MessageSystem::Subject::revoke(ID_type id) {
        m_registry < SUBTYPE >.erase(id);//remove all bound methods for the ID
    };

    template<typename SUBTYPE>//allocate storage
    std::map<ID_type, MessageSystem::Subject::Invocation<SUBTYPE> >  MessageSystem::Subject::m_registry;

}


namespace wlo{
class Messenger : public MessageSystem::Observer{
public:
      Messenger() = default;

      wlo::MessageSystem::Subject & asSubject(){
            return m_subject;
      }

      template<typename T>
      void notify(const T& msg){
          return m_subject.notify<T>(msg);
      }

    template<typename SUBTYPE, typename Obs, void(Obs::*Method)(const SUBTYPE &)>
    void permit(Observer *obs){
          m_subject.permit<SUBTYPE,Obs,Method>(obs);
      }
private:
    MessageSystem::Subject m_subject;
   };

}


#endif
