
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
        FileReader.cpp
        FileReader.h
        Camera.cpp
        Camera.h
        TargetCamera.cpp
        TargetCamera.h
        LevelLoader.cpp
        LevelLoader.h
        ImageLevelLoader.cpp
        ImageLevelLoader.h
        TilesetLevelLoader.cpp
        TilesetLevelLoader.h
        WorldObjectsLevelLoader.cpp
        WorldObjectsLevelLoader.h
        SceneBattle.cpp
        SceneBattle.h
)

add_executable(lightcasting ${SOURCE_FILES})
target_link_libraries(lightcasting PRIVATE ImGui-SFML::ImGui-SFML nlohmann_json::nlohmann_json)