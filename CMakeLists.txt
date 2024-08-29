CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

PROJECT(tosa-roller)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STABDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(src)

ADD_EXECUTABLE(tosa-roller
  src/main.cpp
  src/error/error.cpp
  src/list-entry/list-entry.cpp
  src/role/role.cpp
  src/alignment/alignment.cpp
  src/faction/faction.cpp
  src/group/group.cpp
  src/role-list/role-list.cpp
)

FIND_PACKAGE(CLI11 CONFIG REQUIRED)
TARGET_LINK_LIBRARIES(tosa-roller PRIVATE CLI11::CLI11)

FIND_PACKAGE(nlohmann_json CONFIG REQUIRED)
TARGET_LINK_LIBRARIES(tosa-roller PRIVATE nlohmann_json::nlohmann_json)
