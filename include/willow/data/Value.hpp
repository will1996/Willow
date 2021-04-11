//
// Created by Will Chambers on 3/26/21.
//

#ifndef WILLOW_VALUE_HPP
#define WILLOW_VALUE_HPP
#include"Type.hpp"
#include"willow/utils/Helpers.hpp"
#include<list>

namespace wlo::data{

   class Value{

   public:


      template<typename T>
        Value( T toWrap):
        m_self(CreateUniquePointer<Model<T>>(toWrap))
        { m_members = buildMembers(m_self.get(),Type::of<T>());}

       Value(Type type);//generically realize a type

       Value(const Value & other);

       Value(const Value && other);

        //default constructor initializes a value of type void, with no storage;
       Value();

       Value &operator[](const std::string &);

       Value & operator = (const Value && other) = default;

       Value & operator = (const Value & other);

       template<typename T>
       T get(){
           return *static_cast<T*>(getData());
       }

        void * getData() const {
            return m_self->getData();
        };

       Type getType() const{
           return m_self->getType();
       }

   private:
        Value(Type , void *);//member Value constructor, attach a value to existing storage

       struct Abstract{
        virtual void* getData() = 0;
        virtual Type getType() = 0;
        virtual UniquePointer<Abstract> copy()= 0;
        virtual void Assign(Abstract * ) = 0;
        virtual ~Abstract() = default;
       };

       std::map<std::string,Value> buildMembers(Abstract*,Type);
       template<typename T>
        struct Model: public Abstract{
            explicit Model(T toStore): storedData(std::move(toStore)){}

            Type getType() override{
                return Type::of<T>();
            }

           void* getData()override{
                return &storedData;
            }

            UniquePointer<Abstract> copy()override{
               return CreateUniquePointer<Model<T>>(*this);
            }

             void Assign(Abstract * other) override{
                size_t copySize = Type::of<T>().footprint();
                 assert(other->getType()==Type::of<T>());
                memcpy(&storedData,static_cast<T*>(other->getData()),copySize);
            }


           T storedData;
        };
       template<>
       struct Model<Type>: public Abstract{
            //storage constuctor, creates storage for a composite type
           explicit Model(Type t):type(std::move(t)),storedData(type.footprint()) {}

           Type getType() override{
               return type;
           }

           void* getData()override{
               return storedData.data();
           }

           UniquePointer<Abstract> copy()override{
               return CreateUniquePointer<Model<Type>>(*this);
           }

           void Assign(Abstract * other) override{
               if(type==Type()) {//void type stored here
                   type = other->getType();
                   storedData.resize(type.footprint());
                   size_t copySize = type.footprint();
                   memcpy(storedData.data(), static_cast<byte *>(other->getData()), copySize);
               }
               else{
                   assert(type==other->getType());
                   size_t copySize = type.footprint();
                   memcpy(storedData.data(), static_cast<byte *>(other->getData()), copySize);
               }
           }

           Type type;
           std::vector<byte> storedData;
       };

       //pointer Abstract, used for accesing data
       template<>
       struct Model<void *>: public Abstract{
            Model(Type t, void * src): type(std::move(t)),data(src){};

            void* getData() override{return data;}

            Type getType() override{return type;}

           UniquePointer<Abstract> copy()override{
               return CreateUniquePointer<Model<void *>>(*this);
           }

           void Assign(Abstract * other) override{
                if(type==Type()){//if this is void, then replace me
                    data = other->getData();
                    type = other->getType();
                    size_t copySize = type.footprint();
                    memcpy(data,static_cast<byte*>(other->getData()),copySize);
                }//void
                else {
                    assert(type==other->getType());
                    size_t copySize = type.footprint();
                    memcpy(data, static_cast<byte *>(other->getData()), copySize);
                }
           }

            Type type;
            void* data;
       };



    UniquePointer<Abstract> m_self;
    std::map<std::string,Value >m_members;

   };



}


#endif //WILLOW_VALUE_HPP
