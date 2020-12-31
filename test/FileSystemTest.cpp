#include<filesystem>
#include<iostream>
namespace fs = std::filesystem;
using fs::path;
int main(int arg, char * argv[]){
    path p = fs::current_path();
    std::cout<<"path: " <<p<<std::endl;


}