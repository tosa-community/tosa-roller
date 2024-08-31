AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = tosa-roller
tosa_roller_SOURCES = src/main.cpp src/error/error.cpp src/list-entry/list-entry.cpp src/role/role.cpp src/alignment/alignment.cpp src/faction/faction.cpp src/group/group.cpp src/role-list/role-list.cpp
tosa_roller_CXXFLAGS = $(CLI11_CXXFLAGS) $(JSON_CXXFLAGS) -Isrc -std=c++20
tosa_roller_LDADD = $(CLI11_LIBS) $(JSON_LIBS)
tosa_roller_datadir = ${prefix}/share/${PACKAGE}/data
tosa_roller_data_DATA = data/*
tosa_roller_presetsdir = ${prefix}/share/${PACKAGE}/presets
tosa_roller_presets_DATA = presets/*
