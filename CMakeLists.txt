CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

PROJECT(tosa-roller VERSION 1.0.0)

SET(CMAKE_CXX_STANDARD 20)
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

ADD_COMPILE_DEFINITIONS(PACKAGE="tosa-roller")
ADD_COMPILE_DEFINITIONS(VERSION="${PROJECT_VERSION}")

ADD_SUBDIRECTORY("lib")

TARGET_LINK_LIBRARIES(tosa-roller PRIVATE CLI11::CLI11)

TARGET_LINK_LIBRARIES(tosa-roller PRIVATE nlohmann_json::nlohmann_json)

INCLUDE(GNUInstallDirs)

INSTALL(TARGETS tosa-roller)
INSTALL(DIRECTORY data DESTINATION ${CMAKE_INSTALL_DATADIR})
INSTALL(DIRECTORY presets DESTINATION ${CMAKE_INSTALL_DATADIR})
