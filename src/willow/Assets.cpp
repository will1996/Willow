#include"willow/Assets.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "willow/external/tiny_obj_loader.h"

namespace wlo {
	Assets::Assets(wlo::ScriptEnvironment& env):EngineComponentInstance<Assets>("Assets",this,env)
	{
	}
	TextureHandle Assets::loadTexture(std::string path)
	{
		m_textures.push_back(Texture(path));
		m_textureLookup[m_textures.back().id] = &m_textures.back();
		return TextureHandle(m_textures.back().id, this);
	}

	Texture& Assets::fetchTexture(wlo::ID_type id)
	{
		return *m_textureLookup.at(id);
	}

MeshHandle	Assets::loadMesh(std::string path) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string  err;
		vector<wlo::Index > indices;
		vector<TexturedVertex3D> vertices;
		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), "./", true)) {
			throw std::runtime_error(err);
		}
		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				TexturedVertex3D vertex{};
				vertex.position = {
				 attrib.vertices[3 * index.vertex_index + 0],
				 attrib.vertices[3 * index.vertex_index + 1],
				 attrib.vertices[3 * index.vertex_index + 2]
				};
				vertex.TexCoord = {
				   attrib.texcoords[2 * index.texcoord_index + 0],
				   1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
				vertices.push_back(vertex);
				indices.push_back(indices.size());
			}
		}

		m_meshes.push_back(Mesh(vertices, indices));
		m_meshLookup[m_meshes.back().id] = &m_meshes.back();
		return MeshHandle( m_meshes.back().id,this);
	}

	Mesh& Assets::fetchMesh(wlo::ID_type id)
	{
		return *m_meshLookup.at(id);
	}

	MaterialHandle Assets::loadMaterial(wlo::ID_type textureID, std::string vertexShaderPath, std::string fragmentShaderPath)
	{
		m_materials.push_back(rendering::Material{ .vertexShader = vertexShaderPath,.fragmentShader = fragmentShaderPath, .texture = fetchTexture(textureID) });
		m_materialLookup[m_materials.back().id] = &m_materials.back();
		return MaterialHandle(m_materials.back().id, this);
	}

	rendering::Material& Assets::fetchMaterial(wlo::ID_type id)
	{
		return *m_materialLookup.at(id);
	}


	template<>
		Mesh &Assets::Handle<Mesh>::get(){
			return assets->fetchMesh(id);
			}
	template<>
		rendering::Material &Assets::Handle<rendering::Material>::get(){
			return assets->fetchMaterial(id);
			}
	template<>
		Texture &Assets::Handle<Texture>::get(){
			return assets->fetchTexture(id);
			}

}