#include"willow.hpp"

using namespace wlo;
class Asteroids : public Application {
public:
	Asteroids(std::string argv_0):
		Application(Application::Info{.appName = "Asteroids",.debugging = true,.windowDimensions = {800,800}},argv_0)
	{
	}
	void setup() override {}
	void stepSim(float dt) override {}
	void draw() override {}

};


int main(int argc, char* argv[]){
	wlo::UniquePointer<Application> app = CreateUniquePointer<Asteroids>(argv[0]);
	app->run();
}