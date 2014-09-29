cmake_minimum_required(VERSION 2.8)

project(test CXX)

function(copy_to_build file_name)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${file_name}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${file_name} ${CMAKE_CURRENT_BINARY_DIR}/${file_name}
		MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${file_name})
endfunction()

include(FindPkgConfig)

set(MAIN_HEADERS
	Cube.h
	Debug.h
	Renderer.h
	VBO.h
	CustomVertex.h
)

set(MAIN_SOURCE 
	main.cpp
	Cube.cpp
	Debug.cpp
	Renderer.cpp
	VBO.cpp
	default.vtx
	default.pxl
	texture.png
)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions(-DDEBUG -O0 -ggdb3)
else()
	add_definitions(-O2 -s -march=native)
endif()

add_definitions(-Wextra -Wall -pipe -std=gnu++11)

if(PKG_CONFIG_FOUND)
	message(STATUS "checking for allegro")
	pkg_check_modules(ALLEGRO REQUIRED allegro-debug-5)
	pkg_check_modules(ALLEGRO_MAIN REQUIRED allegro_main-debug-5)
	pkg_check_modules(ALLEGRO_IMAGE REQUIRED allegro_image-debug-5)
	pkg_check_modules(ALLEGRO_PRIMITIVES REQUIRED allegro_primitives-debug-5)
	
	if(ALLEGRO_FOUND AND ALLEGRO_PRIMITIVES_FOUND AND ALLEGRO_IMAGE_FOUND)
		message(STATUS "Allegro ${ALLEGRO_VERSION} was found.")
		add_definitions(${ALLEGRO_CFLAGS})
		message(STATUS "cflags ${ALLEGRO_CFLAGS} libs ${ALLEGRO_LIBRARIES} ${ALLEGRO_MAIN_LIBRARIES} ${ALLEGRO_IMAGE_LIBRARIES} ${ALLEGRO_PRIMITIVES_LIBRARIES}")
		
		copy_to_build(default.vtx)
		copy_to_build(default.pxl)
		copy_to_build(texture.png)
		
		include_directories(${PROJECT_SOURCE_DIR})
		add_executable(main ${MAIN_SOURCE} ${MAIN_HEADERS})
		target_link_libraries(main ${ALLEGRO_LIBRARIES} GL ${ALLEGRO_PRIMITIVES_LIBRARIES} ${ALLEGRO_IMAGE_LIBRARIES})

	else()
		message(ERROR "Allegro library not found")
	endif()
else()
	message(ERROR "pkg-config not found")
endif()

