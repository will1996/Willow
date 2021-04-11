//
// Created by W on 12/5/20.
//

#ifndef WILLOW_TYPE_HPP
#define WILLOW_TYPE_HPP
#include "willow/root/Root.hpp"
    namespace wlo::data {
        template<typename T>
        struct MemberT{
            std::string name;
            T type;
            size_t offset = 0;
        };

        class Type{
        public:
            Type();
            Type(std::string name, size_t size);
            Type(std::string name, std::vector<MemberT<Type>> members);
            Type(std::string name, Type t);


            //two types are compatible if their memory sizes, and the types of all of their fields are the same,
            //but have different member names
            //asks the question, can I de-serialize this data into a struct with Type::of<T>() without faulting?
            bool compatibleWith(const Type & other) const;

            //two types are equal only if they have all of the field same names, and field types
            //do these types represent the same primitive type? or
            //do these types represent the same struct?
            bool operator==(const Type &other) const ;
            [[nodiscard]] bool isPrimitive()const;
            [[nodiscard]] bool isComposite()const;
            [[nodiscard]] bool isContainer()const;
            [[nodiscard]] size_t footprint() const ;
            [[nodiscard]] const std::string name() const;
            [[nodiscard]] const std::vector<MemberT<Type>>  getMembers() const;

            static size_t sizeOf(Type) ;

           template<typename T>
            struct Of{
               Type operator ()();
            };

            template<typename T>
           static Type of(){
               Type t = Of<T>()();
              return t;
           }

        protected:
            size_t _size() const;
            std::string m_name;
            bool m_isPrimitve;
            bool m_isContainer;
            bool m_isComposite;
            size_t m_size;
            std::vector<MemberT<Type>> m_members;
        public:
            using Member  = MemberT<Type>;
        };





    }



inline std::ostream& operator<<(std::ostream & o, const wlo::data::Type&  type){
    o<<"< Data: totalMemorySize: "<<type.footprint()<<std::endl;
    for(auto [name,type, offset] : type.getMembers())
        o<<type<<", offset: "<<offset<<std::endl;
    o<<">";
    return o;
}

namespace std {
  template <>
  struct hash<wlo::data::Type>
  {
    std::size_t operator()(const wlo::data::Type& layout) const
    {
      using std::size_t;
      using std::hash;
      using std::string;
      using wlo::data::Type;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      if(layout.isPrimitive())
          return hash<string>()(layout.name())^((hash<size_t>()(layout.footprint())));

      size_t hash_value = hash<string>()(layout.getMembers()[0].name)^((hash<wlo::data::Type>()(layout.getMembers()[0].type)<<1)>>1) ^((layout.getMembers()[0].offset << 2) >> 2);
          for (size_t i = 1; i < layout.getMembers().size(); i++)
              hash_value ^= hash<string>()(layout.getMembers()[i].name)^((hash<wlo::data::Type>()(layout.getMembers()[i].type)<<(i+1))>>(i+1)) ^ ((layout.getMembers()[i].offset << (i + 2)) >> (i + 2));

    return hash_value;
    }
  };

  template <>
  struct equal_to<wlo::data::Type>
  {
    bool operator()(const wlo::data::Type& one, const wlo::data::Type& two) const
    {
      using namespace wlo;
     if(one.footprint()!=two.footprint() )
         return false;
     if(one.getMembers().size() != two.getMembers().size())
         return false;
    for(size_t i = 0; i< one.getMembers().size(); i++)
        if(one.getMembers()[i].type != two.getMembers()[i].type)
            return false;
   return true;

    }
  };
}

#define RegisterPrimativeType(typename)\
template<>\
struct wlo::data::Type::Of<typename>{  \
wlo::data::Type operator()(){  \
    return wlo::data::Type(#typename,sizeof(typename)); \
    }                                      \
};                                      \

RegisterPrimativeType(unsigned int);
RegisterPrimativeType(float);
RegisterPrimativeType(double);
RegisterPrimativeType(int);
RegisterPrimativeType(bool);
RegisterPrimativeType(char);
RegisterPrimativeType(wlo::byte);

#undef RegisterPrimativeType


template<>
struct wlo::data::Type::Of<std::string>{
    Type operator()(){
        return Type("string",sizeof(std::string));
    }
};

//Register basic non-primitive Willow Types here:



template<>
struct wlo::data::Type::Of<wlo::Vec2>{
    Type operator()(){
        return wlo::data::Type("Vec2",{
                { "x", wlo::data::Type::of<float>()},
                { "y", wlo::data::Type::of<float>()}
        });
    }
};

template<>
struct wlo::data::Type::Of<wlo::Vec3>{
    Type operator()(){
        return wlo::data::Type("Vec3",{
                {"x",wlo::data::Type::of<float>()},
                {"y",wlo::data::Type::of<float>()},
                {"z",wlo::data::Type::of<float>()}
        });
    }
};

template<>
struct wlo::data::Type::Of<wlo::Vec4>{
    Type operator()() {
        return wlo::data::Type("Vec4", {
                {"x", wlo::data::Type::of<float>()},
                {"y", wlo::data::Type::of<float>()},
                {"z", wlo::data::Type::of<float>()},
                {"w", wlo::data::Type::of<float>()}
        });
    }
};


template<>
struct wlo::data::Type::Of<wlo::Mat3>{
    Type operator()() {
        return wlo::data::Type("Mat3",{
                {"r0",wlo::data::Type::of<wlo::Vec3>()},
                {"r1",wlo::data::Type::of<wlo::Vec3>()},
                {"r2",wlo::data::Type::of<wlo::Vec3>()},
        });
    }
};

template<>
struct wlo::data::Type::Of<wlo::Mat4>{
    Type operator()() {
        return wlo::data::Type("Mat4",{
                {"r0",wlo::data::Type::of<wlo::Vec4>()},
                {"r1",wlo::data::Type::of<wlo::Vec4>()},
                {"r2",wlo::data::Type::of<wlo::Vec4>()},
                {"r3",wlo::data::Type::of<wlo::Vec4>()},
        });
    };
};

template<typename T, typename A>
struct wlo::data::Type::Of<std::vector<T,A>>{
    Type operator ()(){
    return Type("Vector Of",{{"Contained",Type::of<T>()}});
    };
};
#endif //WILLOW_TYPE_HPP
