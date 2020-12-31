//
// Created by W on 12/29/20.
//

#ifndef WILLOW_GEOMETRY_HPP
#define WILLOW_GEOMETRY_HPP
#include"Buffer.hpp"
namespace wlo::rendering{
   class Geometry3D{
       Geometry3D(Buffer<Vertex3D>);
   };

   class IndexedGeometry :Geometry{
       IndexedGeometry(Buffer<Vertex2D>)
   };


}


#endif //WILLOW_GEOMETRY_HPP
