#include"Entity.hpp"
#include"root/EngineComponent.hpp"
#include"rendering/Material.hpp"
#include"rendering/Mesh.hpp"
#include"Assets.hpp"

namespace wlo{
	
	class Game:public EngineComponentInstance<Game>{
	public:	
		Game(wlo::ScriptEnvironment& env,Assets & assets);

		wlo::ID_type spawnEnt(Assets::MeshHandle , Assets::MaterialHandle);

	private:
		Assets& m_assets;
	};


}

