//
// Created by W on 12/5/20.
//

#ifndef WILLOW_DATALAYOUT_HPP
#define WILLOW_DATALAYOUT_HPP
#include "willow/root/wilo_dev_core.hpp"
namespace  wlo::rendering{
    class DataLayout{
    public:
        enum class DataType {
            Float, Double, Int, Long,  Bool
        };

       struct Element{
            DataType type;
            size_t count;
       };


        DataLayout(std::initializer_list<Element> elements);
        DataLayout(std::initializer_list<const DataLayout> layouts);

        size_t memSize() const ;

        std::vector<std::pair<Element,size_t> >  get() const;

        static size_t sizeOf(DataType) ;
        static size_t sizeOf(Element) ;

        template<typename T>
        static DataLayout fromFundamental();

        template<>
         DataLayout fromFundamental<float>(){
            return DataLayout({{DataType::Float,1}});
        }

        template<>
        DataLayout fromFundamental<double>(){
            return DataLayout({{DataType::Double,1}});
        }

        template<>
        DataLayout fromFundamental<size_t>(){
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



#endif //WILLOW_DATALAYOUT_HPP
