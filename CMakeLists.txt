#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(geant4template)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
#
find_package(Geant4 REQUIRED ui_all vis_all)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(geant4template geant4template.cc ${sources} ${headers})
target_link_libraries(geant4template ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory. This is so that we can run the
# executable directly because it relies on these scripts being in the current
# working directory.
#
set(SCRIPTS
  run.mac
  vis.mac
  )

foreach(_script ${SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
