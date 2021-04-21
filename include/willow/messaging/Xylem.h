#ifndef UNTITLED_ZYLEM_H
#define UNTITLED_ZYLEM_H
#include <utility>
#include<unordered_map>
#include<typeindex>
#include<vector>
#include<cassert>
#include<stdexcept>
class Xylem{
public:
    typedef size_t ID;

    template <class T>
    struct Message  {
        Message(T idata):data(idata),id(s_id++){};
        const T data;
        const ID id;
    private:
        static ID s_id;
    };



    //primary methods
    template<class T, class U, class V> static void buildStaticChannel(U* reciever){ s_core.buildChannel<T,U,V>(reciever); }
    template<class T, class U, class V> static void sendStaticDirect(const Message<V> & msg){ s_core.sendMessage<T,U,V>(msg); }


private:
    template< class T,class U,class V>
    struct Channel{
        U* target;
    };

   class Core{
   public:
       template<class T, class U, class V >
           constexpr void buildChannel(U* target ){
               if(s_directChannels < T,U, V >.target)
                   throw std::runtime_error("attempted to overwrite an existing channel, static channels may only be created once");
           s_directChannels < T,U, V >.target = target;
           }

       template<class T, class U, class V>
    constexpr void sendMessage(const Message<V> & msg ){
               if(!s_directChannels < T,U, V >.target)
                   throw std::runtime_error("attempted to send a message on a static channel with no reciever. Recievers must build a channel to recieve messages");
           s_directChannels < T,U, V >.target->recieve(msg);
           }

   private:
       template<class T, class U,class V>
        static Channel<T,U,V>  s_directChannels;
   };

    static Core s_core;



};


template<class T>
Xylem::ID Xylem::Message<T>::s_id;

template<class T, class U, class V>
Xylem::Channel<T,U,V> Xylem::Core::s_directChannels;


#endif //UNTITLED_ZYLEM_H
