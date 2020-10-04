#pragma once
#include<iostream>
namespace wlo{
   typedef  enum class  MouseButton : uint16_t{
        NONE         =   0,
        BUTTON1      =   1,
        BUTTON2      =   2,
        BUTTON3      =   3,
        BUTTON4      =   4,
        BUTTON5      =   5,
        BUTTON6      =   6,
        BUTTON7      =   7,
        BUTTON8      =   8,
        BUTTONLAST   =   BUTTON8,
        BUTTONLEFT   =   BUTTON1,
        BUTTONRIGHT  =   BUTTON2,
        BUTTONMIDDLE =   BUTTON3,

    }Mouse;

    inline std::ostream& operator<<(std::ostream& os, Mouse m){
        os<< static_cast<int32_t>(m);
        return os;
    }



}