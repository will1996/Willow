#include "willow/messaging/wilo_subject.hpp"
#include "willow/messaging/wilo_observer.hpp"
#include "willow/messaging/wilo_message.hpp"



namespace wlo{
     Subject::Subject(){}
     Subject::Subject(ID_type id ): EngineElement(id){}

     void Subject::initialize()
     {
     }

     void Subject::reclaim()
     {
     }

    //add and remove observers from internal observers map
    //definition moved to header to see if that resolves linker issues
//        template<typename O,typename M, void(O::*Method)(M& m) >
//    void Subject::permit(O* obs);
//        m_observers[obs->getId()] = obs;
//
//    }
//    void Subject::revoke(Observer* obs){
//        throw std::runtime_error("Unimplemented");
//    }
//    void Subject::notifyAll(Message& message){
//        throw std::runtime_error("Uniplemented");
//    }
        
}