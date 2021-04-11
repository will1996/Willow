//
// Created by Will Chambers on 3/2/21.
//
#include <willow/messaging/MessageSystem.hpp>
using namespace wlo;
#include <string>
using std::string;
class Gossip : public Messenger{
public:

    struct Whisper{
        string msg;
    };
    Gossip(string Name):name(Name){}
    void hearGossip(const Whisper & msg){
        memory.push_back(msg.msg);
    }
    void sayGossip(const string & msg){

       Whisper whisper{
           .msg = name+" says: "+ msg
       };
        Messenger::asSubject().notify(whisper);
    }

    std::vector<string> memory;
    const string name;

};

int main(){
    Gossip dale("dale");
    Gossip greg("greg");

    greg.permit<Gossip::Whisper,Gossip,&Gossip::hearGossip>(&dale);
    dale.permit<Gossip::Whisper,Gossip,&Gossip::hearGossip>(&greg);

    dale.sayGossip("did you see who was on dancing with the stars last night?");
    greg.sayGossip("lady gaga");

    if(greg.memory[0]!="dale says: did you see who was on dancing with the stars last night?"){
        cout<<"Messenger test failed, message should have been: "<<"dale says: did you see who was on dancing with the stars last night?"<<"was: "<<greg.memory[0]<<endl;
        return -1;
    }
    if(dale.memory[0]!="dale says: did you see who was on dancing with the stars last night?"){
        cout<<"Messenger test failed, message should have been: "<<"greg says: lady gaga"<<"was: "<<dale.memory[0]<<endl;
        return -1;
    }

}