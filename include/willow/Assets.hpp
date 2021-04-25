#include"root/EngineComponent.hpp"
#include"rendering/Mesh.hpp"
#include"rendering/Material.hpp"
#include"rendering/Texture.hpp"
#include"root/FileSystem.hpp"
#pragma once
namespace wlo {
	class Assets : public EngineComponentInstance<Assets> {
	public:
		template<typename T>
		struct Handle{
			wlo::ID_type id;
			T& get();
			Handle(wlo::ID_type ID, Assets* pAssets):id(ID),assets(pAssets){}
			Handle():id(0),assets(nullptr){
			}
		private:
			Assets* assets;
		};
		
		Handle<Mesh> TexturedQuad();
        Handle<Mesh> TexturedQuad(std::vector<wlo::Vec2> customTextureCoords);
		Handle<Mesh> TexturedCube();
			
				
		
		
	

		Assets(wlo::ScriptEnvironment& env);

		Handle<Texture> loadTexture(std::string path);
		Texture& fetchTexture(wlo::ID_type);

		Handle<Mesh> loadMesh(std::string path);
		Handle<Mesh> takeMesh(Mesh && mesh);

		Mesh& fetchMesh(wlo::ID_type);
		
		Handle<rendering::Material> loadMaterial(wlo::ID_type textureID,
								  std::string vertexShaderPath,
			                      std::string fragmentShaderPath);
		rendering::Material& fetchMaterial(wlo::ID_type id);


	private:
		std::list<Mesh> m_meshes;
		std::list<Texture> m_textures;
		std::list<rendering::Material> m_materials;



		std::unordered_map<wlo::ID_type, Texture*> m_textureLookup;
		std::unordered_map<wlo::ID_type, Mesh*> m_meshLookup;
		std::unordered_map<wlo::ID_type, rendering::Material*> m_materialLookup;

	};

		using TextureHandle = Assets::Handle<Texture>;
		using MeshHandle = Assets::Handle<Mesh>;
		using MaterialHandle = Assets::Handle<rendering::Material>;



}
