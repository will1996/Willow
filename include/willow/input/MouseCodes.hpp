#pragma once
#include<iostream>
#include<string>
namespace wlo{
    class Mouse {
        public:
        enum class Code : uint16_t {
            NONE = 0,
            BUTTON1 = 1,
            BUTTON2 = 2,
            BUTTON3 = 3,
            BUTTON4 = 4,
            BUTTON5 = 5,
            BUTTON6 = 6,
            BUTTON7 = 7,
            BUTTON8 = 8,
            BUTTONLAST = BUTTON8,
            BUTTONLEFT = BUTTON1,
            BUTTONRIGHT = BUTTON2,
            BUTTONMIDDLE = BUTTON3,
        };
        static inline std::string ButtonName( const Code & code){
            return getMouseCodeName[code];
        }
    private:
        inline static std::map<Mouse::Code,std::string> getMouseCodeName = {
                {Mouse::Code::NONE,      "NONE"},
                {Mouse::Code::BUTTON1,     "BUTTON1"},
                {Mouse::Code::BUTTON2,     "BUTTON2"},
                {Mouse::Code::BUTTON3,     "BUTTON3"},
                {Mouse::Code::BUTTON4,     "BUTTON4"},
                {Mouse::Code::BUTTON5,     "BUTTON5"},
                {Mouse::Code::BUTTON6,     "BUTTON6"},
                {Mouse::Code::BUTTON7,     "BUTTON7"},
                {Mouse::Code::BUTTON8,     "BUTTON8"},
                {Mouse::Code::BUTTONLAST,   "BUTTONLAST"},
                {Mouse::Code::BUTTONLEFT,  "BUTTONLEFT"},
                {Mouse::Code::BUTTONRIGHT, "BUTTONRIGHT"},
                {Mouse::Code::BUTTONMIDDLE,"BUTTONMIDDLE"}
        };
    };
}
inline std::ostream &operator<<(std::ostream &os, wlo::Mouse::Code code) {
    os << wlo::Mouse::ButtonName(code);
    return os;
}
