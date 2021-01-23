//
// Created by W on 12/5/20.
//

#ifndef WILLOW_DATALAYOUT_HPP
#define WILLOW_DATALAYOUT_HPP
#include "willow/root/wilo_dev_core.hpp"
namespace  wlo::rendering{
    class DataLayout{
    public: enum class DataType:size_t {
            Byte = 0,Float = 1, Double = 2, Int = 3, Long = 4,  Bool = 5
        };

       struct Element{
            DataType type;
            size_t count;
            inline bool operator ==(Element other) {
                return type == other.type && count == other.count;
            }
       };

        DataLayout();
        explicit DataLayout(std::initializer_list<Element> elements);
        explicit DataLayout(std::initializer_list<const DataLayout> layouts);
        explicit DataLayout(std::vector<Element> elements);
        inline bool operator==(DataLayout other) {
            if (m_memSize != other.m_memSize)
                return false;
            if (m_elementList.size() != other.m_elementList.size())
                return false;

            for (size_t i = 0; i < m_elementList.size(); i++) {
                if (m_elementList[i].first != other.m_elementList[i].first)
                    return false;
                if (m_elementList[i].second != other.m_elementList[i].second)
                    return false;
            }

                return true;
        }
        size_t memSize() const ;

        std::vector<std::pair<Element,size_t> >  get() const;

        static size_t sizeOf(DataType) ;
        static size_t sizeOf(Element) ;

        template<typename T>
        static DataLayout fromFundamental();

        template<>
        static  DataLayout fromFundamental<float>(){
            return DataLayout({{DataType::Float,1}});
        }

        template<>
        static DataLayout fromFundamental<double>(){
            return DataLayout({{DataType::Double,1}});
        }

        template<>
        static DataLayout fromFundamental<size_t>(){
            return DataLayout({{DataType::Long,1}});
        }
        

    private:
        size_t m_memSize;
        std::vector<std::pair<Element,size_t> > m_elementList;
    };


}

inline std::ostream& operator<<(std::ostream & o,const wlo::rendering::DataLayout::DataType& type) {
    switch (type) {
        case wlo::rendering::DataLayout::DataType::Float:
            o<<"Float";
            break;
        case wlo::rendering::DataLayout::DataType::Double:
            o<<"Double";
            break;
        case wlo::rendering::DataLayout::DataType::Int:
            o<<"Int";
            break;
        case wlo::rendering::DataLayout::DataType::Long:
            o<<"Long";
            break;
        case wlo::rendering::DataLayout::DataType::Bool:
            o<<"Bool";
            break;
    }
    return o;
}

inline std::ostream& operator<<(std::ostream & o, const wlo::rendering::DataLayout::Element&  element){
        o<<"{ type: "<<element.type<<", count: "<<element.count<<"}";
        return o;
    }

inline std::ostream& operator<<(std::ostream & o, const wlo::rendering::DataLayout&  layout){
    o<<"< DataLayout: totalMemorySize: "<<layout.memSize()<<std::endl;
    for(auto [element, offset] : layout.get())
        o<<element<<", offset: "<<offset<<std::endl;
    o<<">";
    return o;
}

inline bool operator==(const wlo::rendering::DataLayout::Element & one, const wlo::rendering::DataLayout::Element & two){
        return one.count==two.count && one.type==two.type;
}


inline bool operator==(const wlo::rendering::DataLayout & one, const wlo::rendering::DataLayout & two){
     if(one.memSize()!=two.memSize() && one.memSize())
         return false;
     if(one.get().size()!=two.get().size())
         return false;
    for(size_t i = 0; i< one.get().size(); i++)
        if(one.get()[i].first!=two.get()[i].first)
            return false;
   return true;
}

namespace std {
  template <>
  struct equal_to<wlo::rendering::DataLayout::Element>
  {
    bool operator()(const wlo::rendering::DataLayout::Element& lhs,const wlo::rendering::DataLayout::Element& rhs) const
    {
        return lhs.count==rhs.count && lhs.type==rhs.type;
    }
  };

  template <>
  struct hash<wlo::rendering::DataLayout::Element>
  {
    std::size_t operator()(const wlo::rendering::DataLayout::Element& el) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      return hash<size_t>()(el.count) ^ ((hash<size_t>()((size_t)el.type)>>1)<<1);
    }
  };



  template <>
  struct hash<wlo::rendering::DataLayout>
  {
    std::size_t operator()(const wlo::rendering::DataLayout& layout) const
    {
      using std::size_t;
      using std::hash;
      using std::string;
      using wlo::rendering::DataLayout;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:
      size_t hash_value = hash<DataLayout::Element>()(layout.get()[0].first) ^ ((layout.get()[0].second << 1) >> 1);
          for (size_t i = 1; i < layout.get().size(); i++)
              hash_value ^= hash<DataLayout::Element>()(layout.get()[0].first) ^ ((layout.get()[0].second << i+1) >> i+1);

    return hash_value;
    }
  };

  template <>
  struct equal_to<wlo::rendering::DataLayout>
  {
    bool operator()(const wlo::rendering::DataLayout& one,const wlo::rendering::DataLayout& two) const
    {
      using namespace wlo::rendering;
     if(one.memSize()!=two.memSize() && one.memSize())
         return false;
     if(one.get().size()!=two.get().size())
         return false;
    for(size_t i = 0; i< one.get().size(); i++)
        if(one.get()[i].first!=two.get()[i].first)
            return false;
   return true;

    }
  };
}

#endif //WILLOW_DATALAYOUT_HPP
