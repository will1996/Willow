//
// Created by Will Chambers on 2/27/21.
//
#include"willow.hpp"
#include<iostream>
#include <vector>
#include<string>
using namespace std;


void printHelp(){
   cout<<"Available commands: "<<endl;
   cout<<"project -> creates a new project in the current directory"<<endl;
}


void parseProjectOptions(vector<string> commandStack){

}

void parseProjectCommand(vector<string> commandStack){
    vector<string> parsedCommandStack(commandStack.begin()+1,commandStack.end());
    string name = commandStack[0];
    parseProjectOptions(parsedCommandStack);

}


void parseCommandRoot(vector<string> commandStack){
    vector<string> parsedCommandStack(commandStack.begin()+1,commandStack.end());
    string rootCommand = commandStack[0];
    if(rootCommand=="project")
        parseProjectCommand(parsedCommandStack);
    else{
       printHelp();
       exit(0);
    }




}
std::vector<string> buildCommandStack(int argc, const char** argv){
    vector<string> commandStack(argc);
    for(size_t i = 0; i < commandStack.size();i++)
        commandStack[i] = string(argv[i]);
    return commandStack;
}

int main(int argc, const char ** argv){
   cout<<"Welcome to Willow" <<endl;
   parseCommandRoot(buildCommandStack(argc,argv));
}
