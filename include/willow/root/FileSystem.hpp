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
		inline static fs::path Willow() { return Root().parent_path().append("Willow"); }
	private:
		static fs::path m_root;
	};

}
