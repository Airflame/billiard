cmake_minimum_required(VERSION 3.16)
project(billiard)

set(CMAKE_CXX_STANDARD 14)

set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/libs/SFML-2.5.1/lib/cmake/SFML")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
if(SFML_FOUND)
    add_executable(billiard main.cpp src/Ball.cpp include/Ball.h src/Cane.cpp include/Cane.h src/Hole.cpp include/Hole.h
            src/Text.cpp include/Text.h src/VectorMath.cpp include/VectorMath.h src/GameServer.cpp
            include/GameServer.h src/GameClient.cpp include/GameClient.h)
    target_link_libraries(billiard sfml-graphics sfml-window sfml-system sfml-network)
endif()