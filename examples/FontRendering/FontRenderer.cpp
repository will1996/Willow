//
// Created by W on 10/9/20.
//
#include"willow.hpp"
using namespace wlo;

class Terminal : public Application {
public:
	Terminal(std::string argv_0):
		Application(argv_0)
	{
	}
	void setup() {


	}
	void stepSim(float dt) {}
	void draw() {}

};


int main(int argc, char* argv[]){
	wlo::UniquePointer<Application> app = CreateUniquePointer<Terminal>(argv[0]);
	app->run();
}