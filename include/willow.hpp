#include"willow/Application.hpp"

#define WILLOW_TRUNK(T) int main(int argv, char** argc){ T app(argc[0]); if(app.isInitialized()){}else app.initialize(); app.run();}
