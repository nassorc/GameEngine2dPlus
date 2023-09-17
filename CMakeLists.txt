cmake_minimum_required(VERSION 3.26)
project(lightcasting)

#set(CMAKE_CXX_STANDARD 17)
#
#add_executable(lightcasting main.cpp)
#
#project(comp400-a4-migrated)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES main.cpp
        GameEngine.h
        EntityManager.h
        Components.h
        Animation.h
        Animation.cpp
        Entity.h
        Action.h
        EntityManager.cpp
        GameEngine.cpp
        Assets.cpp
        Assets.h
        Physics.h
        Scene.cpp
        Scene.h
        SceneMenu.cpp
        SceneMenu.h
        Vec2.cpp
        Vec2.h
        ScenePlay.cpp
        ScenePlay.h
        Physics.cpp
        GameEngine.cpp
        ScenePlayground.cpp
        ScenePlayground.h
        SceneParallax.cpp
        SceneParallax.h
)
add_executable(lightcasting ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})


target_link_libraries(lightcasting sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
