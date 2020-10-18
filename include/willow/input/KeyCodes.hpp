#pragma once
#include<iostream> 

namespace wlo{
    class Key {
    public:
        enum class Code {
            UNKNOWN = -1,
            SPACE = 32,
            APOSTROPHE = 39,  /* ' */
            COMMA = 44,  /* , */
            MINUS = 45,  /* - */
            PERIOD = 46,  /* . */
            SLASH = 47,  /* / */
            D0 = 48,
            D1 = 49,
            D2 = 50,
            D3 = 51,
            D4 = 52,
            D5 = 53,
            D6 = 54,
            D7 = 55,
            D8 = 56,
            D9 = 57,
            SEMICOLON = 59,  /* ; */
            EQUAL = 61,  /* = */
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            LEFT_BRACKET = 91,  /* [ */
            BACKSLASH = 92,  /* \ */
            RIGHT_BRACKET = 93,  /* ] */
            GRAVE_ACCENT = 96,  /* ` */
            WORLD_1 = 161, /* non-US #1 */
            WORLD_2 = 162, /* non-US #2 */
            ESCAPE = 256,
            ENTER = 257,
            TAB = 258,
            BACKSPACE = 259,
            INSERT = 260,
            DELETE = 261,
            RIGHT = 262,
            LEFT = 263,
            DOWN = 264,
            UP = 265,
            PAGE_UP = 266,
            PAGE_DOWN = 267,
            HOME = 268,
            END = 269,
            CAPS_LOCK = 280,
            SCROLL_LOCK = 281,
            NUM_LOCK = 282,
            PRINT_SCREEN = 283,
            PAUSE = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,
            KP_0 = 320,
            KP_1 = 321,
            KP_2 = 322,
            KP_3 = 323,
            KP_4 = 324,
            KP_5 = 325,
            KP_6 = 326,
            KP_7 = 327,
            KP_8 = 328,
            KP_9 = 329,
            KP_DECIMAL = 330,
            KP_DIVIDE = 331,
            KP_MULTIPLY = 332,
            KP_SUBTRACT = 333,
            KP_ADD = 334,
            KP_ENTER = 335,
            KP_EQUAL = 336,
            LEFT_SHIFT = 340,
            LEFT_CONTROL = 341,
            LEFT_ALT = 342,
            LEFT_SUPER = 343,
            RIGHT_SHIFT = 344,
            RIGHT_CONTROL = 345,
            RIGHT_ALT = 346,
            RIGHT_SUPER = 347,
            MENU = 348
        };

        enum Modifier : uint16_t {
            NONE = 0x000,
            SHIFT = 0x0001,
            CONTROL = 0x0002,
            ALT = 0x0004,
            SUPER = 0x0008,
            CAPS_LOCK = 0x0010,
            NUM_LOCK = 0x0020,

        };
       static inline std::string toText(const Code & code,const Modifier& mod = Modifier::NONE ) {
        if(isLetter(code)||isDigit(code)||isSymbol(code))
               if(mod==Modifier::SHIFT)
                   return std::string{getUppercase[code]};
               else
                   return std::string{getLowerCase[code]};
        else
            return getKeyName[code];
       }

        static std::string getBundleName(uint16_t bundle){
            std::stringstream name;
            name<<"|";
            if (bundle&Key::Modifier::ALT)
                name<<"ALT|";
            if (bundle&Key::Modifier::SHIFT)
                name<<"SHIFT|";
            if (bundle&Key::Modifier::SUPER)
                name<<"SUPER|";
            if (bundle&Key::Modifier::CONTROL)
                name<<"CONTROL|";
            if (bundle&Key::Modifier::CAPS_LOCK)
                name<<"CAPS_LOCK|";
            if (bundle&Key::Modifier::NUM_LOCK)
                name<<"NUM_LOCK|";
            return name.str();
       }

        static inline bool hasModifer(const uint16_t & bundle, const Modifier& mod) {
           return (bundle&mod);
        }

        static inline bool isSymbol(const Code & code){
            switch(code) {
                case Code::PERIOD:
                    return true;
                case Code::COMMA:
                    return true;
                case Code::SLASH:
                    return true;
                case Code::BACKSLASH:
                    return true;
                case Code::LEFT_BRACKET:
                    return true;
                case Code::RIGHT_BRACKET:
                    return true;
                case Code::EQUAL:
                    return true;
                case Code::MINUS:
                    return true;
                case Code::GRAVE_ACCENT:
                    return true;
                case Code::SEMICOLON:
                    return true;
                case Code::APOSTROPHE:
                    return true;
                default:
                    return false;
            }
        }


        static inline bool isDigit(const Code & code){
            switch(code) {
                case Code::D0:
                    return true;
                case Code::D1:
                    return true;
                case Code::D2:
                    return true;
                case Code::D3:
                    return true;
                case Code::D4:
                    return true;
                case Code::D5:
                    return true;
                case Code::D6:
                    return true;
                case Code::D7:
                    return true;
                case Code::D8:
                    return true;
                case Code::D9:
                    return true;
                default:
                    return false;
            }
        }
        static inline bool isLetter(const Code & code){
            switch(code) {
                case Code::A:
                    return true;
                case Code::B:
                    return true;
                case Code::C:
                    return true;
                case Code::D:
                    return true;
                case Code::E:
                    return true;
                case Code::F:
                    return true;
                case Code::G:
                    return true;
                case Code::H:
                    return true;
                case Code::I:
                    return true;
                case Code::J:
                    return true;
                case Code::K:
                    return true;
                case Code::L:
                    return true;
                case Code::M:
                    return true;
                case Code::N:
                    return true;
                case Code::O:
                    return true;
                case Code::P:
                    return true;
                case Code::Q:
                    return true;
                case Code::R:
                    return true;
                case Code::S:
                    return true;
                case Code::T:
                    return true;
                case Code::U:
                    return true;
                case Code::V:
                    return true;
                case Code::W:
                    return true;
                case Code::X:
                    return true;
                case Code::Y:
                    return true;
                case Code::Z:
                    return true;
                default:
                    return false;
            }
        }

        static inline bool isModifierKey(const Code & code){
           switch (code){
               case Code::LEFT_SHIFT:
                   return true;
               case Code::RIGHT_SHIFT:
                   return true;
               case Code::RIGHT_ALT:
                   return true;
               case Code::LEFT_ALT:
                   return true;
               case Code::LEFT_CONTROL:
                   return true;
               case Code::RIGHT_CONTROL:
                   return true;
               default:
                   return false;
           }
       }

    private:

        inline static std::map<Key::Code, char> getUppercase = {
                {Key::Code::A,             'A'},
                {Key::Code::B,             'B'},
                {Key::Code::C,             'C'},
                {Key::Code::D,             'D'},
                {Key::Code::E,             'E'},
                {Key::Code::F,             'F'},
                {Key::Code::G,             'G'},
                {Key::Code::H,             'H'},
                {Key::Code::I,             'I'},
                {Key::Code::J,             'J'},
                {Key::Code::K,             'K'},
                {Key::Code::L,             'L'},
                {Key::Code::M,             'M'},
                {Key::Code::N,             'N'},
                {Key::Code::O,             'O'},
                {Key::Code::P,             'P'},
                {Key::Code::Q,             'Q'},
                {Key::Code::R,             'R'},
                {Key::Code::S,             'S'},
                {Key::Code::T,             'T'},
                {Key::Code::U,             'U'},
                {Key::Code::V,             'V'},
                {Key::Code::W,             'R'},
                {Key::Code::X,             'X'},
                {Key::Code::Y,             'Y'},
                {Key::Code::Z,             'Z'},
                {Key::Code::D0,            ')'},
                {Key::Code::D1,            '!'},
                {Key::Code::D2,            '@'},
                {Key::Code::D3,            '#'},
                {Key::Code::D4,            '$'},
                {Key::Code::D5,            '%'},
                {Key::Code::D6,            '^'},
                {Key::Code::D7,            '&'},
                {Key::Code::D8,            '*'},
                {Key::Code::D9,            '('},
                {Key::Code::PERIOD,        '>'},
                {Key::Code::COMMA,         '<'},
                {Key::Code::LEFT_BRACKET,  '{'},
                {Key::Code::APOSTROPHE,    '"'},
                {Key::Code::RIGHT_BRACKET, '}'},
                {Key::Code::GRAVE_ACCENT,  '~'},
                {Key::Code::SLASH,         '?'},
                {Key::Code::BACKSLASH,     '|'},
                {Key::Code::MINUS,         '_'},
                {Key::Code::EQUAL,         '+'},
                {Key::Code::TAB,           '\t'}
        };
        inline static std::map<Key::Code, char> getLowerCase = {
                {Key::Code::A,             'a'},
                {Key::Code::B,             'b'},
                {Key::Code::C,             'c'},
                {Key::Code::D,             'd'},
                {Key::Code::E,             'e'},
                {Key::Code::F,             'f'},
                {Key::Code::G,             'g'},
                {Key::Code::H,             'h'},
                {Key::Code::I,             'i'},
                {Key::Code::J,             'j'},
                {Key::Code::K,             'k'},
                {Key::Code::L,             'l'},
                {Key::Code::M,             'm'},
                {Key::Code::N,             'n'},
                {Key::Code::O,             'o'},
                {Key::Code::P,             'p'},
                {Key::Code::Q,             'q'},
                {Key::Code::R,             'r'},
                {Key::Code::S,             's'},
                {Key::Code::T,             't'},
                {Key::Code::U,             'u'},
                {Key::Code::V,             'v'},
                {Key::Code::W,             'r'},
                {Key::Code::X,             'x'},
                {Key::Code::Y,             'y'},
                {Key::Code::Z,             'z'},
                {Key::Code::D0,            '0'},
                {Key::Code::D1,            '1'},
                {Key::Code::D2,            '2'},
                {Key::Code::D3,            '3'},
                {Key::Code::D4,            '4'},
                {Key::Code::D5,            '5'},
                {Key::Code::D6,            '6'},
                {Key::Code::D7,            '7'},
                {Key::Code::D8,            '8'},
                {Key::Code::D9,            '9'},
                {Key::Code::PERIOD,        '.'},
                {Key::Code::COMMA,         ','},
                {Key::Code::LEFT_BRACKET,  '['},
                {Key::Code::RIGHT_BRACKET, ']'},
                {Key::Code::GRAVE_ACCENT,  '`'},
                {Key::Code::APOSTROPHE,    '\''},
                {Key::Code::SLASH,         '/'},
                {Key::Code::BACKSLASH,     '\\'},
                {Key::Code::MINUS,         '-'},
                {Key::Code::EQUAL,         '='},
                {Key::Code::TAB,           '\t'}
        };

        inline static std::map<Key::Code,std::string> getKeyName = {
                {Key::Code::UNKNOWN , "UNKNOWN"},
                {Key::Code::SPACE , "SPACE"},
                {Key::Code::APOSTROPHE , "APOSTROPHE"},  /* ' */
                {Key::Code::COMMA , "COMMA"},  /* , */
                {Key::Code::MINUS , "MINUS"},  /* - */
                {Key::Code::PERIOD , "PERIOD"},  /* . */
                {Key::Code::SLASH , "SLASH"},  /* / */
                {Key::Code::D0 , "D0"},
                {Key::Code::D1 , "D1"},
                {Key::Code::D2 , "D2"},
                {Key::Code::D3 , "D3"},
                {Key::Code::D4 , "D4"},
                {Key::Code::D5 , "D5"},
                {Key::Code::D6 , "D6"},
                {Key::Code::D7 , "D7"},
                {Key::Code::D8 , "D8"},
                {Key::Code::D9 , "D9"},
                {Key::Code::SEMICOLON , "SEMICOLON"} , /* ; */
                {Key::Code::EQUAL , "EQUAL"} , /* = */
                {Key::Code::A , "A"},
                {Key::Code::B , "B"},
                {Key::Code::C , "C"},
                {Key::Code::D , "D"},
                {Key::Code::E , "E"},
                {Key::Code::F , "F"},
                {Key::Code::G , "G"},
                {Key::Code::H , "H"},
                {Key::Code::I , "I"},
                {Key::Code::J , "J"},
                {Key::Code::K , "K"},
                {Key::Code::L , "L"},
                {Key::Code::M , "M"},
                {Key::Code::N , "N"},
                {Key::Code::O , "O"},
                {Key::Code::P , "P"},
                {Key::Code::Q , "Q"},
                {Key::Code::R , "R"},
                {Key::Code::S , "S"},
                {Key::Code::T , "T"},
                {Key::Code::U , "U"},
                {Key::Code::V , "V"},
                {Key::Code::W , "W"},
                {Key::Code::X , "X"},
                {Key::Code::Y , "Y"},
                {Key::Code::Z , "Z"},
                {Key::Code::LEFT_BRACKET ,  "LEFT_BRACKET"} , /* [ */
                {Key::Code::BACKSLASH ,     "BACKSLASH"} , /* \ */
                {Key::Code::RIGHT_BRACKET , "RIGHT_BRACKET"} , /* ] */
                {Key::Code::GRAVE_ACCENT ,  "GRAVE_ACCENT"} , /* ` */
                {Key::Code::WORLD_1 ,       "WORLD_1"} ,/* non-US #1 */
                {Key::Code::WORLD_2 ,       "WORLD_2"} ,/* non-US #2 */
                {Key::Code::ESCAPE ,        "ESCAPE"},
                {Key::Code::ENTER ,         "ENTER"},
                {Key::Code::TAB ,           "TAB"},
                {Key::Code::BACKSPACE , "BACKSPACE"},
                {Key::Code::INSERT ,    "INSERT"},
                {Key::Code::DELETE ,    "DELETE"},
                {Key::Code::RIGHT ,     "RIGHT"},
                {Key::Code::LEFT ,      "LEFT"},
                {Key::Code::DOWN ,      "DOWN"},
                {Key::Code::UP ,        "UP"},
                {Key::Code::PAGE_UP ,   "PAGE_UP"},
                {Key::Code::PAGE_DOWN , "PAGE_DOWN"},
                {Key::Code::HOME ,      "HOME"},
                {Key::Code::END ,       "END"},
                {Key::Code::CAPS_LOCK , "CAPS_LOCK"},
                {Key::Code::SCROLL_LOCK,"SCROLL_LOCK"},
                {Key::Code::NUM_LOCK ,  "NUM_LOCK"},
                {Key::Code::PRINT_SCREEN , "PRINT_SCREEN"},
                {Key::Code::PAUSE , "PAUSE"},
                {Key::Code::F1 ," F1 "},
                {Key::Code::F2 ," F2 "},
                {Key::Code::F3 ," F3 "},
                {Key::Code::F4 ," F4 "},
                {Key::Code::F5 ," F5 "},
                {Key::Code::F6 ," F6 "},
                {Key::Code::F7 ," F7 "},
                {Key::Code::F8 ," F8 "},
                {Key::Code::F9 ," F9 "},
                {Key::Code::F10," F10" },
                {Key::Code::F11," F11" },
                {Key::Code::F12," F12" },
                {Key::Code::F13," F13" },
                {Key::Code::F14," F14" },
                {Key::Code::F15," F15" },
                {Key::Code::F16," F16" },
                {Key::Code::F17," F17" },
                {Key::Code::F18," F18" },
                {Key::Code::F19," F19" },
                {Key::Code::F20," F20" },
                {Key::Code::F21," F21" },
                {Key::Code::F22," F22" },
                {Key::Code::F23," F23" },
                {Key::Code::F24," F24" },
                {Key::Code::F25," F25" },
                {Key::Code::KP_0 , "KP_0"},
                {Key::Code::KP_1 , "KP_1"},
                {Key::Code::KP_2 , "KP_2"},
                {Key::Code::KP_3 , "KP_3"},
                {Key::Code::KP_4 , "KP_4"},
                {Key::Code::KP_5 , "KP_5"},
                {Key::Code::KP_6 , "KP_6"},
                {Key::Code::KP_7 , "KP_7"},
                {Key::Code::KP_8 , "KP_8"},
                {Key::Code::KP_9 , "KP_9"},
                {Key::Code::KP_DECIMAL ,  "KP_DECIMAL"},
                {Key::Code::KP_DIVIDE ,   "KP_DIVIDE"},
                {Key::Code::KP_MULTIPLY , "KP_MULTIPLY"},
                {Key::Code::KP_SUBTRACT , "KP_SUBTRACT"},
                {Key::Code::KP_ADD ,      "KP_ADD"},
                {Key::Code::KP_ENTER ,    "KP_ENTER"},
                {Key::Code::KP_EQUAL ,    "KP_EQUAL"},
                {Key::Code::LEFT_SHIFT ,  "LEFT_SHIFT"},
                {Key::Code::LEFT_CONTROL ,"LEFT_CONTROL"},
                {Key::Code::LEFT_ALT ,    "LEFT_ALT"},
                {Key::Code::LEFT_SUPER ,  "LEFT_SUPER"},
                {Key::Code::RIGHT_SHIFT , "RIGHT_SHIFT"},
                {Key::Code::RIGHT_CONTROL , "RIGHT_CONTROL"},
                {Key::Code::RIGHT_ALT , "RIGHT_ALT"},
                {Key::Code::RIGHT_SUPER , "RIGHT_SUPER"},
                {Key::Code::MENU  ,"MENU"}
        };



    };
    inline std::ostream &operator<<(std::ostream &os, wlo::Key::Code k) {
        os << static_cast<int32_t>(k);
        return os;

    }
    inline std::ostream &operator<<(std::ostream &os, wlo::Key::Modifier k) {
        os << static_cast<int32_t>(k);
        return os;
    }
}
