//
// Created by Will Chambers on 8/18/21.
//

#include "willow/data/Data.hpp"


namespace wlo {
#define RegisterPrimativeType(typename)\
template<>SharedPointer<const data::Type> wlo::Data::typeOf<typename> = CreateSharedPointer<const data::Type>(#typename,sizeof(typename)); \

    RegisterPrimativeType(unsigned int);
    RegisterPrimativeType(float);
    RegisterPrimativeType(double);
    RegisterPrimativeType(int);
    RegisterPrimativeType(bool);
    RegisterPrimativeType(char);
    RegisterPrimativeType(uint8_t);
    RegisterPrimativeType(wlo::byte);
#undef RegisterPrimativeType
    template<>
    SharedPointer<const data::Type> wlo::Data::typeOf<void> = CreateSharedPointer<const data::Type>();
    template<>
    SharedPointer<const data::Type>wlo::Data::typeOf<std::string> = CreateSharedPointer<const data::Type>(data::Type("string", sizeof(std::string)));
    template<>
    SharedPointer<const data::Type>wlo::Data::typeOf<wlo::Vec2> = CreateSharedPointer<const data::Type>(data::Type("Vec2", {
            { "x",  wlo::Data::typeOf<float>},
            {"y", wlo::Data::typeOf<float>}}));
    template<>
    SharedPointer<const data::Type>wlo::Data::typeOf<wlo::Vec3> = CreateSharedPointer<const data::Type>(data::Type("Vec3", {
            {"x", wlo::Data::typeOf<float>},
            {"y", wlo::Data::typeOf<float>},
            {"z", wlo::Data::typeOf<float>},
    }));
    template<>
    SharedPointer<const data::Type>wlo::Data::typeOf<wlo::Vec4> = CreateSharedPointer<const data::Type>(data::Type("Vec4", {
            {"x", wlo::Data::typeOf<float>},
            {"y", wlo::Data::typeOf<float>},
            {"z", wlo::Data::typeOf<float>},
            {"w", wlo::Data::typeOf<float>},
    }));
    template<>
    SharedPointer<const data::Type>wlo::Data::typeOf<wlo::Mat3> = CreateSharedPointer<const data::Type>(data::Type("Mat3", {
            {"r0", wlo::Data::typeOf<Vec3>},
            {"r1", wlo::Data::typeOf<Vec3>},
            {"r2", wlo::Data::typeOf<Vec3>},
    }));
    template<>
    SharedPointer<const data::Type>wlo::Data::typeOf<wlo::Mat4> = CreateSharedPointer<const data::Type>(data::Type("Mat4", {
            {"r0", wlo::Data::typeOf<Vec4>},
            {"r1", wlo::Data::typeOf<Vec4>},
            {"r2", wlo::Data::typeOf<Vec4>},
            {"r3", wlo::Data::typeOf<Vec4>},
    }));


}