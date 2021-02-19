#ifndef WILLOW_MATERIAL_HPP
#define WILLOW_MATERIAL_HPP
#include "willow/root/Root.hpp"
#include "willow/root/Tag.hpp"
#include "willow/root/FileSystem.hpp"
namespace wlo::rendering {

    struct Material : Tag {
        std::string vertexShader;
        std::string fragmentShader;
        std::string texture;
    };
}
#endif
