#include"root/Root.hpp"
#include"rendering/Mesh.hpp"
#include"rendering/Material.hpp"
#include "Assets.hpp"

namespace wlo {
	struct Ent:Tag{
		wlo::Mat4 transform;
		Assets::MeshHandle mesh;
		Assets::MaterialHandle Material;
	};
}