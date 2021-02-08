#include "willow/messaging/MessageSystem.hpp"
#include "willow/messaging/Messages.hpp"
#include"willow/rendering/PrespectiveCamera3D.hpp"
#include"willow/root/wilo_dev_core.hpp"
#include <functional>
namespace wlo{
   template<class T> 
   class Controller: wlo::MessageSystem::Observer{
      public:
         Controller(wlo::MessageSystem::Subject & inputSource, T & peon,std::map<wlo::Key::Code,std::function<void(T&)>> actionMap):
         m_peon(peon),
         m_actionMap(actionMap)
            {
               inputSource.permit<KeyboardMessage,Controller<T>,&Controller<T>::applyAction>(this);
            }
      private:
         void applyAction(const wlo::KeyboardMessage & msg){
               m_actionMap[msg.content.button] (m_peon);
         }
         T & m_peon;
         std::map<wlo::Key::Code,std::function<void(T&)>> m_actionMap;
   }; 





}
