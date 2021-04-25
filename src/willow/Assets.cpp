#include"willow/Assets.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "willow/external/tiny_obj_loader.h"

namespace wlo {

	Mesh buildQuad(){
		std::vector<wlo::TexturedVertex3D> verts = {
			{{-.5,-.5,0},{0,0}},
			{{.5,-.5,0},{1,0}},
			{{.5,.5,0},{1,1}},
			{{-.5,.5,0},{0,1}},
		};
		std::vector<wlo::Index> inds = {
			0,1,2,
			2,3,0
		};
		return Mesh(verts, inds);
	}

    Mesh buildQuad(std::vector<wlo::Vec2> customTexCoords){
	    assert(customTexCoords.size()==4);
        std::vector<wlo::TexturedVertex3D> verts = {
                {{-.5,-.5,0},customTexCoords[0]},
                {{.5,-.5,0},customTexCoords[1]},
                {{.5,.5,0},customTexCoords[2]},
                {{-.5,.5,0},customTexCoords[3]},
        };
        std::vector<wlo::Index> inds = {
                0,1,2,
                2,3,0
        };
        return Mesh(verts, inds);
    }

	Mesh buildCube(){
    std::vector < wlo::TexturedVertex3D> vertices = {
    
               { .position = {-1.0f, 1.0f, -1.0},.TexCoord = {0,0}},
               { .position = {1.0f, 1.0f, -1.0},.TexCoord = {1,0}},
               { .position = {1.0f, -1.0f, -1.0},.TexCoord = {1,1}},
               { .position = {-1.0f, -1.0f, -1.0},.TexCoord = {0,1}},

               { .position = {-1.0f, 1.0f, 1.0},.TexCoord = {0,1}},
               { .position = {1.0f, 1.0f, 1.0},.TexCoord = {1,1}},
               { .position = {1.0f, -1.0f, 1.0},.TexCoord = {1,0}},
               { .position = {-1.0f, -1.0f, 1.0},.TexCoord = {0,0}},
    };

    std::vector<wlo::Index> indices = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3


    };


    return wlo::Mesh(vertices, indices);

	}


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

	MeshHandle Assets::takeMesh(Mesh&& mesh)
	{
		m_meshes.push_back(std::move(mesh));
		m_meshLookup[m_meshes.back().id] = &m_meshes.back();
		return MeshHandle( m_meshes.back().id,this);
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

	MeshHandle Assets::TexturedQuad()
	{
		return takeMesh(buildQuad());
	}
	
	MeshHandle Assets::TexturedCube()
	{
		return takeMesh(buildCube());
	}

    Assets::Handle<Mesh> Assets::TexturedQuad(std::vector<wlo::Vec2> customTextureCoords) {
	    return takeMesh(buildQuad(customTextureCoords));
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