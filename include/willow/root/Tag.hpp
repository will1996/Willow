//
// Created by W on 12/10/20.
//

#ifndef WILLOW_TAG_HPP
#define WILLOW_TAG_HPP
#include "willow/root/Root.hpp"
namespace wlo{
struct Tag {
    static size_t counter;
    wlo::ID_type id;
    Tag() : id(counter++) {}
    Tag(ID_type _id):id(_id){}
};
}
#endif //WILLOW_TAG_HPP
