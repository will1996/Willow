#include"willow/Assets.hpp";

#include"TestingTools.hpp"


using namespace wlo;
int main(){

	ScriptEnvironment env;

	Assets assets(env);

	TextureHandle tex = assets.loadTexture(FileSystem::Texture("cow.bmp").string()); 
	require(tex.get().depth() == 4);

}
