cmake_minimum_required(VERSION 3.10)

set(GAME_ENGINE_SOURCES src/main.cpp src/inputsManager.cpp src/gameWorld.cpp src/blob.cpp)
set(GAME_ENGINE_HEADERS include/inputsManager.hpp include/gameWorld.hpp include/blob.hpp)

add_executable(GameEngine ${GAME_ENGINE_SOURCES} ${GAME_ENGINE_HEADERS})

target_link_libraries(GameEngine PUBLIC physicslib opengl_wrapperlib)

file(COPY ${PROJECT_SOURCE_DIR}/resources DESTINATION .)