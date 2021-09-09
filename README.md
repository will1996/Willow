# Willow_cpp
A from-scratch learning engine written in modern C++. 

Features: 
* Compile time resolved messaging system, reduces cost of message sending to that of a single function call. 
* Runtime reflection system, generalizes to any user type with a single macro invocation
* Type erased Values created using the reflection system, emulates dynamic typing when you want it. 
* Decoupled Message passing architecture for easy extension 
* Lua Scripting
* Compiletime specified ECS with scriptable entity creation, component modification, system definition and control
* Vulkan Rendering + Compute API (Currently under heavy revision) 
* CMake based asset management + shader compilation, performs path transformations and packaging at build time, delivering portable executables complete with assets
* Message based logging + Error handling enables clear and flexible debugging from script or from c++

Up next: 
* Rework Vulkan backend; expose command buffer creation as primary graphics API - should enable easy and flexible graphics interface to ECS
* Add audio support

Willow has been the backing engine for two game jams now! 

https://willchambers.itch.io/ld48-rabbit-hole

https://willchambers.itch.io/ld-47-disasteroids







