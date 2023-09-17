
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
        Index.h
)

add_executable(lightcasting ${SOURCE_FILES})
target_link_libraries(lightcasting PRIVATE ImGui-SFML::ImGui-SFML )