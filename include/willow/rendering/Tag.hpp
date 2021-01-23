//
// Created by W on 12/10/20.
//

#ifndef WILLOW_TAG_HPP
#define WILLOW_TAG_HPP
#include "willow/root/wilo_dev_core.hpp"
#include"willow/root/wilo_object_counter.hpp"
namespace wlo::rendering{
struct Tag {
    static size_t counter;
    wlo::ID_type id;
    Tag() : id(counter++) {}
};
}
#endif //WILLOW_TAG_HPP
