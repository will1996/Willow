#include "willow/messaging/wilo_observer.hpp"
#include "willow/messaging/wilo_subject.hpp"


namespace wlo{
    Observer::Observer(){};
    Observer::Observer(ID_type id ): EngineElement(id){};
    void Observer::reclaim(){
        for(auto *sub : m_subjects){
            sub->revoke(this->getID());
        }
    }
    void Observer::initialize()
    {
    }
    Observer::~Observer(){
        
    }


}