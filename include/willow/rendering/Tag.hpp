//
// Created by W on 12/10/20.
//

#ifndef WILLOW_TAG_HPP
#define WILLOW_TAG_HPP
#include "willow/root/wilo_dev_core.hpp"
namespace wlo::rendering{
struct Tag {
    static size_t counter;
    size_t id;
    Tag() : id(counter++) {}
};
}
#endif //WILLOW_TAG_HPP
