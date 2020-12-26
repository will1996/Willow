//
// Created by W on 10/9/20.
//

#include "willow/console/FontMap.hpp"







namespace wlo{

   FontMap::FontMap() { }


    FontMap::Character FontMap::build_Q() {
       std::vector<LineSegment> segments = {
               {{1.5,.5},{.5,1.5}} ,
               {{.5,1.5},{.75,1.25}} ,
               {{.75,1.25},{.75,1.25}} ,
       };
       throw std::runtime_error("UNIMPLEMENTED");
    }

    FontMap::Character FontMap::build_0() {
        std::vector<LineSegment> segments = {
                //inner loop
                {{2.5,2},{2,2.5}} ,
                {{2,2.5},{2,6.5}} ,
                {{2,6.5},{2.5,7}} ,
                {{2.7,7},{7,7}} ,
                {{7,7},{7.5,6.5}} ,
                {{7.5,6.5},{7.5,2.5}} ,
                {{7.5,2.5},{7,2}} ,
                {{7,2},{2.5,2}} ,
                //outer loop
                {{2,1},{1,2}} ,
                {{1,2},{1,7}} ,
                {{1,7},{2,8}} ,
                {{2,8},{8,8}} ,
                {{8,8},{9,7}} ,
                {{9,7},{9,2}} ,
                {{9,2},{8,1}} ,
                {{8,1},{2,1}}
        };
        throw std::runtime_error("UNIMPLEMENTED");
    }


}