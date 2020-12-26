//
// Created by W on 10/20/20.
//

#ifndef WILLOW_OBJECTS_HPP
#define WILLOW_OBJECTS_HPP
#include"willow/root/wilo_dev_core.hpp"
#include"willow/rendering/wilo_vertex_types.hpp"
namespace wlo::Bloom{
    struct Pipeline{};
    struct Draw{};
    struct Model{
        const std::string name;
    private:
        friend class GraphicsSystem;
        Model(std::string name,UniquePointer<std::vector<Vertex3D>>, UniquePointer<std::vector<uint32_t> > );
    };
    struct Frame{};
}



#endif //WILLOW_OBJECTS_HPP
