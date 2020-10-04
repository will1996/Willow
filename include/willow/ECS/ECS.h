//
// Created by W on 9/25/20.
//

#ifndef GROVE_ECS_H
#define GROVE_ECS_H
#include"/root/wilo_dev_core.hpp"
namespace wlo{
    typedef size_t EntityHandle;



   class ECS;




   template<class T>
    class ComponentStack{
    public:
        ComponentStack(wilo::SharedPointer<ECS>){


        }

    private:
        std::vector<T> m_components;
        std::unordered_map<EntityHandle,T> components;

    };

   class ECS {
   public:
       ECS(ECS*) {};

       template<class T>
               ComponentStack<T>& allocateComponentStack(EntityHandle ent){
                    s_ComponentMap<T>[this] = ComponentStack<T>(wilo::CreateSharedPointer<ECS>(this));
               }
   private:
       template<class T>
          static   std::unordered_map< ECS* ,ComponentStack<T>  > s_ComponentMap;


   };


}


#endif //GROVE_ECS_H
