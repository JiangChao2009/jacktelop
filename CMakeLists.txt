cmake_minimum_required(VERSION 2.8)
project(jacktelop_sp)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "default install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

include(ExternalProject)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules/)
configure_file(${PROJECT_SOURCE_DIR}/cmake/Findlcm.cmake.in ${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules/Findlcm.cmake @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/cmake/FindROS.cmake.in ${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules/FindROS.cmake @ONLY)
if( USERNAME )
else()
  set( USERNAME $ENV{USER} )
  message( "setting USERNAME to ${USERNAME}" )
endif()

find_package(lcm)
if( NOT lcm_FOUND )
  message("lcm not found, installing")
  ExternalProject_Add(lcm
    PREFIX lcm
    DOWNLOAD_COMMAND git clone https://github.com/lcm-proj/lcm.git lcm 
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./bootstrap.sh && ./configure --prefix=${CMAKE_INSTALL_PREFIX}
    BUILD_COMMAND ${MAKE}
  )
else()
  message("lcm found, not installing")
  add_custom_target(lcm)
endif()

ExternalProject_Add(jacktelop
  DEPENDS lcm
  PREFIX jacktelop
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/src
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_MODULE_PATH=${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules -DBUILD_DOCS:BOOL=${BUILD_DOCS}
)
ExternalProject_Get_Property(jacktelop binary_dir)
ExternalProject_Add_Step(jacktelop fb
  ALWAYS 1
  DEPENDERS build
)
install(TARGETS)
