AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = tosa-roller
tosa_roller_SOURCES = src/main.cpp src/error/error.cpp src/list-entry/list-entry.cpp src/role/role.cpp src/alignment/alignment.cpp src/faction/faction.cpp
tosa_roller_CXXFLAGS = $(CLI11_CXXFLAGS) $(JSON_CXXFLAGS) -Isrc
tosa_roller_LDADD = $(CLI11_LIBS) $(JSON_LIBS)
