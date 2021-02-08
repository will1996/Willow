#pragma once
#include<string>
#include<filesystem>
namespace fs = std::__fs::filesystem;
namespace wlo {
	class FileSystem {
	public: 
		inline static fs::path Root() { return fs::current_path().parent_path(); }
	private:
	};

}
