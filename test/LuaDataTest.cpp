//
// Created by Will Chambers on 3/17/21.
//
#include"willow/scripting/LuaBinding.hpp"
#include"willow/scripting/LuaCommunication.hpp"
#include "TestingTools.hpp"
using namespace wlo;
class LuaTester{
    public:
   explicit LuaTester(ScriptEnvironment &env):
   binding("LuaTester",this,env)
   {
        binding.Register<Vec3,&LuaTester::CallMe>("CallMe");
       binding.Register<std::string,&LuaTester::CallMe>("CallMeString");
       binding.Register<std::vector<Vec3> ,&LuaTester::CallMe>("CallMePoints");
       binding.Register<std::string,int ,&LuaTester::CallMe>("CallMeMulti");
       binding.Register<std::string,int,float ,&LuaTester::CallMe>("CallMeMulti2");
       binding.Register<int,wlo::Vec3 ,&LuaTester::CallMe>("CallMeMulti3");
       binding.Register<std::string,wlo::Vec3 ,&LuaTester::CallMe>("CallMeMulti4");


   }

private:
    void CallMe(Vec3 i){
       cout<<"you called with value "<<glm::to_string(i)<<endl;
   }
   void CallMe(std::string name){
       cout<<"you called with name "<<name<<endl;
   }

    void CallMe(std::string name,int number){
        cout<<"you called with name "<<name<<"and a number: "<<number<<endl;
    }

    void CallMe(std::string name,int number,float number2){
        cout<<"you called with name "<<name<<"and a number: "<<number<<"and another number"<<number2<<endl;
    }

    void CallMe(int number,wlo::Vec3 vec){
        cout<<"you called with number"<<number<<"and a vector"<<glm::to_string(vec)<<endl;
    }
    void CallMe(std::string name,wlo::Vec3 vec){
        cout<<"you called with name"<<name<<"and a vector"<<glm::to_string(vec)<<endl;
    }

    void CallMe(std::vector<Vec3> points){
        cout<<"you called with a vector with:  "<<points.size()<<"elements"<<endl;
        size_t i =0;
        for(auto & element:points)
            cout<<"\t element: "<<i++<<" = "<<glm::to_string(element)<<endl;
    }
   LuaBinding<LuaTester> binding;
};

int main(){
    wlo::logr::initalize();
    ScriptEnvironment env;
    LuaTester tester(env);

    lua::Stack stack(env.getL());
    float x = 34.0f;
    stack.push(x);
    auto x2 = stack.pop(data::Type::of<float>()).get<float>();
    require(x==x2);

    int y = 34;
    stack.push(y);
    auto y2 = stack.pop(data::Type::of<int>()).get<int>();
    require(y==y2);

    std::string z = "thirty four";
    stack.push(z);
    auto z2 = stack.pop(data::Type::of<std::string>()).get<std::string>();
    require(z==z2);

    cout<<"empty check -1"<<endl;
    require(stack.isEmpty());

    wlo::Vec3 vec = {0,1,2};
    env.setGlobal("testVec",vec);
    env.setGlobal("testInt",y);
    env.setGlobal("testFloat",x);
    env.setGlobal("testString",z);
    cout<<"stack after setting globals"<<endl;
    stack.print();
    env.runScript(R"(
            print("testInt "..testInt)
            print("testFloat "..testFloat)
            print("testString "..testString)
            print("testVec, Size:  "..#testVec)
            print("testVec.x:  "..testVec.x)
            print("testVec.y:  "..testVec.y)
            print("testVec.z:  "..testVec.z)
        )");
    cout<<"empty check -.5"<<endl;
    require(stack.isEmpty());

    stack.push(vec);
    auto backVec = stack.pop(data::Type::of<wlo::Vec3>()).get<wlo::Vec3>();

    cout<<"empty check 0"<<endl;
    require(stack.isEmpty());

    require(backVec==vec);
    auto globalBackVec = env.getGlobal<wlo::Vec3>("testVec");
    stack.print();
    require(globalBackVec==vec);
    cout<<"empty check 1"<<endl;

    require(stack.isEmpty());

    env.runScript(R"(LuaTester:CallMeString("hello"))");

    env.runScript(R"(
        Player = {}
        Player.name = "Theodor"
        Player.age = 35
)");
    std::string PlayerName = env.getTableEntry<std::string>("Player","name");
    require(PlayerName=="Theodor");

    env.runScript(R"(LuaTester:CallMe({x=30.0,y=45.0,z=22.0}))");
    env.runScript(R"(LuaTester:CallMeMulti("hey there",40))");
    env.runScript(R"(LuaTester:CallMeMulti2("hey there",40,33.33))");
    env.runScript(R"(LuaTester:CallMeMulti3(40,{x=33.33,y=40.0,z=55.0}))");
    env.runScript(R"(LuaTester:CallMeMulti4("named vec",{x=33.33,y=40.0,z=55.0}))");
}