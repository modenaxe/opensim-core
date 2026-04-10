project(MuscleForceDirection)

cmake_minimum_required(VERSION 3.2)

# OpenSim uses C++20 language features.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSim REQUIRED PATHS "${OPENSIM_INSTALL_DIR}")

# header and sources
set(MuscleForceDirection_HEADERS
  MuscleForceDirection.h RegisterTypes_osimPlugin.h osimPluginDLL.h)
set(MuscleForceDirection_SOURCES
  MuscleForceDirection.cpp RegisterTypes_osimPlugin.cpp)
source_group("MuscleForceDirection_headers"
  FILES ${MuscleForceDirection_HEADERS})
source_group("MuscleForceDirection_sources"
  FILES ${MuscleForceDirection_SOURCES})

# library
set(target MuscleForceDirection)
add_library(${target} SHARED
  ${MuscleForceDirection_HEADERS}
  ${MuscleForceDirection_SOURCES})
target_link_libraries(${target} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target} PROPERTIES
  DEFINE_SYMBOL OSIMPLUGIN_EXPORTS
  PROJECT_LABEL "Libraries - MuscleForceDirection")
install(TARGETS ${target}
    RUNTIME DESTINATION plugins
    LIBRARY DESTINATION plugins)
