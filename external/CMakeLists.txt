include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0
        GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.88
        GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(ImGui)

set(IMGUI_DIR "${imgui_SOURCE_DIR}")
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)
FetchContent_Declare(ImGui-SFML
        GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml
        GIT_TAG v2.6
        GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(ImGui-SFML)

add_library(imgui_lib
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp")
#add_library(ImPlot::ImPlot ALIAS ImPlot)
target_include_directories(imgui_lib PUBLIC "${imgui_SOURCE_DIR}")