#include "Tag.hpp"
#include"willow/scripting/LuaBinding.hpp"
#include"willow/messaging/MessageSystem.hpp"
#pragma once
namespace wlo {
	class EngineComponent :public Messenger {
	protected:
		EngineComponent(ScriptEnvironment &env):m_scriptEnv(env){}
		ScriptEnvironment& m_scriptEnv;
	};

	template<typename T>
	class EngineComponentInstance:public EngineComponent{
	public:
		EngineComponentInstance(std::string name,T* instance,ScriptEnvironment & env):EngineComponent(env),m_scriptBinding(name,instance,env){ }

	private:
		LuaBinding<T> m_scriptBinding;
	};

}