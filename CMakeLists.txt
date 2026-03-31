cmake_minimum_required(VERSION 4.1)
project(martian-miner)

set(CMAKE_CXX_STANDARD 20)

#Set SDL3 include and lib paths
if (WIN32)
    set(SDL3_PATH "${PROJECT_SOURCE_DIR}/src/vendor/SDL3-3.2.20")
    set(SDL3_IMAGE_PATH "${PROJECT_SOURCE_DIR}/src/vendor/SDL3_image-3.2.4")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/src/ecs
            ${PROJECT_SOURCE_DIR}/src/ecs/system
            ${PROJECT_SOURCE_DIR}/src/utils
            ${PROJECT_SOURCE_DIR}/src/vendor
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
    )
endif ()

add_executable(martian-miner src/main.cpp
        src/Game.cpp
        src/managers/TextureManager.h
        src/managers/TextureManager.cpp
        src/Map.h
        src/Map.cpp
        src/ecs/ComponentType.h
        src/ecs/Entity.h
        src/ecs/Component.h
        src/ecs/system/MovementSystem.h
        src/ecs/system/RenderSystem.h
        src/ecs/World.h
        src/utils/Vector2D.h
        src/ecs/system/KeyboardInputSystem.h
        src/utils/Vector2D.cpp
        src/utils/Collision.h
        src/utils/Collision.cpp
        src/ecs/system/CollisionSystem.h
        src/ecs/system/CollisionSystem.cpp
        src/ecs/events/EventManager.h
        src/ecs/World.cpp
        src/vendor/tinyxml2.h
        src/vendor/tinyxml2.cpp
        src/Map.cpp
        src/ecs/system/AnimationClip.h
        src/managers/AssetManager.h
        src/managers/AssetManager.cpp
        src/ecs/system/AnimationSystem.h
        src/ecs/system/CameraSystem.h
        src/ecs/system/SpawnTimerSystem.h
        src/ecs/system/DestructionSystem.h
        src/managers/Scene.h
        src/managers/Scene.cpp
        src/managers/SceneManager.h
        src/ecs/system/AccelerationSystem.h
        src/ecs/system/VelocitySystem.h
        src/scenes/LandingScene.cpp
        src/scenes/LandingScene.h
        src/ecs/events/BaseEvent.h
        src/ecs/system/EventResponseSystem.h
        src/ecs/system/EventResponseSystem.cpp
)

target_link_libraries(martian-miner
        SDL3
        SDL3_IMAGE
)