//
// Created by Will Chambers on 8/20/21.
//

#ifndef WILLOW_TYPEOF_HPP
#define WILLOW_TYPEOF_HPP

#ifndef WILLOW_TYPE_HPP
#include "Type.hpp"
#endif
#define RegPrimitive(typename) \
template<> struct TypeOf<typename> { \
 static Type get(){            \
 return Type(#typename,sizeof(typename));\
 }                              \
};
#define RegOneMemberType(typename,membername,membertype )\
template<> struct TypeOf<typename> { \
 static Type get(){            \
 return Type(#typename,{{membername,TypeOf<membertype>::get()}});\
 }                              \
};
#define RegTwoMemberType(typename,membername1,membertype1,membername2,membertype2 )\
template<> struct TypeOf<typename> { \
static Type get(){            \
 return Type(#typename,{                                                           \
 {membername1,TypeOf<membertype1>::get()},                        \
 {membername2,TypeOf<membertype2>::get()},                        \
 });\
 }                              \
};
#define RegThreeMemberType(typename,membername1,membertype1,membername2,membertype2 ,membername3,membertype3)\
template<> struct TypeOf<typename> { \
 static Type get(){            \
 return Type(#typename,{                                                           \
 {membername1,TypeOf<membertype1>::get()},                        \
 {membername2,TypeOf<membertype2>::get()},                        \
 {membername3,TypeOf<membertype3>::get()},                        \
 });\
 }                              \
};
#define RegFourMemberType(typename,membername1,membertype1,membername2,membertype2 ,membername3,membertype3,membername4,membertype4)\
template<> struct TypeOf<typename> { \
 static Type get(){            \
 return Type(#typename,{                                                           \
 {membername1,TypeOf<membertype1>::get()},                        \
 {membername2,TypeOf<membertype2>::get()},                        \
 {membername3,TypeOf<membertype3>::get()},                        \
 {membername4,TypeOf<membertype4>::get()},                        \
 });\
 }                              \
};

namespace wlo::data{
   template<typename T>
   struct TypeOf{ };
    template<typename T>
    Type typeOf() {
        return TypeOf<T>::get();
    }
#ifndef WILLOW_TYPEOF_SPECIALIZATIONS
#define  WILLOW_TYPEOF_SPECIALIZATIONS
   template<>
   struct TypeOf<void>{
       static Type get(){
           return Type();
       }
   };
   RegPrimitive(std::string)
   RegPrimitive(unsigned int)
   RegPrimitive(float)
   RegPrimitive(double)
   RegPrimitive(int)
   RegPrimitive(bool)
   RegPrimitive(char)
   RegPrimitive(uint8_t)
   RegPrimitive(wlo::byte)
#undef RegPrimitive
    RegTwoMemberType(Vec2,"x",float,"y",float)
    RegThreeMemberType(Vec3,"x",float,"y",float,"z",float)
    RegFourMemberType(Vec4,"x",float,"y",float,"z",float,"w",float)
    RegThreeMemberType(Mat3,"r0",Vec3,"r1",Vec3,"r2",Vec3)
    RegFourMemberType(Mat4,"r0",Vec4,"r1",Vec4,"r2",Vec4,"r3",Vec4)
    RegTwoMemberType(TexturedVertex3D,"position",Vec3,"tex",Vec2)

    RegTwoMemberType(Pixel2,"r",byte,"g",byte)
    RegThreeMemberType(Pixel3,"r",byte,"g",byte,"b",byte)
    RegFourMemberType(Pixel4,"r",byte,"g",byte,"b",byte,"a",byte)


#endif // TypeOf specializations
    template<typename T, typename A>
    struct TypeOf<std::vector<T,A>>{
        static Type get(){
            return Type("Vector of",{{"contained",TypeOf<T>::get()}});
        }
    };
}


#endif //WILLOW_TYPEOF_HPP
