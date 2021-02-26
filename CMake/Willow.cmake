cmake_minimum_required(VERSION 3.16)

function add_willow_executable(name source)
	add_executable(name source EXCLUDE_FROM_ALL)	
	target_link_libraries(name PUBLIC Willow)
	file(COPY )