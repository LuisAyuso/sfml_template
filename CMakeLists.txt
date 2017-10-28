
cmake_minimum_required (VERSION 3.8)
project(myapp CXX)

list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules/)
find_package(SFML 2 REQUIRED network audio graphics window system)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)


set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(SRCS src/main.cpp)

add_executable(myapp ${SRCS})

set_property(TARGET myapp 
    PROPERTY 
        CXX_STANDARD 17
)

target_include_directories(myapp 
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/includes
        ${SFML_INCLUDE_DIR}
        ${X11_INCLUDE_DIR}
)

target_link_libraries(myapp 
        ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} 
        pthread OpenGL::GL
        ${X11_LIBRARIES}
)

