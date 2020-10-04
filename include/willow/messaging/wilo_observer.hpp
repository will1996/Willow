#pragma once
#include "willow/root/wilo_dev_core.hpp"
#include "willow/root/wilo_object_counter.hpp"
#include "wilo_message.hpp"
#include "willow/root/wilo_engine_element.hpp"
namespace wlo{
    class Subject;
class Observer  : public EngineElement{
   friend class Subject;
   public:
       Observer();
       Observer(ID_type id);
    
    void reclaim() override;
    void initialize() override;
    virtual ~Observer();

    ID_type  getID(){
        return m_ID;
    }
    
   private:
    //we need this list of subjects to notify when this obererver is freed, this is how we populate that list.
    void trackSubject(Subject* sub){
        m_subjects.push_back(sub);
    }
    std::vector<Subject*> m_subjects;
};
}