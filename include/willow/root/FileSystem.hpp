#pragma once
#include<string>
#include<filesystem>
namespace fs = std::filesystem;
namespace wlo {
	class FileSystem {
	public: 
		inline static void initialize() {
		m_root = fs::current_path();
		};

		inline static void initialize(fs::path root) {
			m_root = root;
		};
		inline static fs::path Root() { return m_root; }
		inline static fs::path Assets() { return Root().append("Assets");}

        inline static fs::path Shaders() { return Root().append("Assets").append("Shaders");}
        inline static fs::path Shader(std::string shaderName) { return Root().append("Assets").append("Shaders").append(shaderName+".spv");}

        inline static fs::path Textures() { return Root().append("Assets").append("Textures");}
        inline static fs::path Texture(std::string textureName) { return Root().append("Assets").append("Textures").append(textureName);}

        inline static fs::path Scripts() { return Root().append("Assets").append("Scripts");}
        inline static fs::path Script(std::string scriptName) { return Root().append("Assets").append("Scripts").append(scriptName);}

		inline static fs::path Willow() { return Root().append("Willow"); }
	private:
		static fs::path m_root;
	};

}
