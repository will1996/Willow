#include"willow.hpp"

using namespace wlo;
class Asteroids : public Application {
public:
	Asteroids(std::string argv_0):
	    Application(argv_0)
	{
	}
	void setup() {}
	void stepSim(float dt) {}
	void draw() {}

};


int main(int argc, char* argv[]){
	wlo::UniquePointer<Application> app = CreateUniquePointer<Asteroids>(argv[0]);
	app->run();
}