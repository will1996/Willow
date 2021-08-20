#pragma once
#include "Tag.hpp"
#include"willow/scripting/LuaBinding.hpp"
#include"willow/messaging/MessageSystem.hpp"
namespace wlo {
	class EngineComponent :public Messenger {
	public:
       virtual void connect(Messenger * comp) {};
	   EngineComponent() = default;
	};


}