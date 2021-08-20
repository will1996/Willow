//
// Created by Will Chambers on 3/29/21.
//

#ifndef WILLOW_DATA_HPP
#define WILLOW_DATA_HPP
#include"Type.hpp"
namespace wlo{
   class Data{

   public:
       template<typename T>
        static void registerType(data::Type typeDesc){
            typeOf<T> =  typeDesc;
        }

       template<typename T>
       static const data::Type & type(){
            return *typeOf<T>;
       }
       template<typename T>
       static const SharedPointer<const data::Type> & ptype(){
           return typeOf<T>;
       }

   private:
       template<typename T>
        static SharedPointer<const data::Type> typeOf;
   };

   template<typename T>
   SharedPointer<const data::Type> Data::typeOf;

   template<typename T, typename A>
   SharedPointer<const data::Type> Data::typeOf<std::vector<T,A> > = CreateSharedPointer<const data::Type>("Vector Of", {{"Contained",Data::typeOf<T>}});
}
#endif //WILLOW_DATA_HPP
