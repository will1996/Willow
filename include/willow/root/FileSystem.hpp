#include<string>
namespace wlo {
	class FileSystem {
	public: 
		inline static std::string Root() { return s_root; }
	private:
		inline static const std::string s_root= "C:\\Users\\sacer\\Source\\Repos\\will1996\\Willow\\" ;


	};

}