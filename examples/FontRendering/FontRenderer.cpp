//
// Created by W on 10/9/20.
//
#include"willow.hpp"
using namespace wlo;

class Terminal : public Application {
public:
	Terminal(std::string argv_0):
		Application(Application::Info{.appName = "Willow Editor",.debugging = true,.windowDimensions = {800,800}},argv_0)
	{
	}
	void setup() override {


	}
	void stepSim(float dt) override {}
	void draw() override {}

};


int main(int argc, char* argv[]){
	wlo::UniquePointer<Application> app = CreateUniquePointer<Terminal>(argv[0]);
	app->run();
}