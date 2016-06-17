cmake_minimum_required(VERSION 2.8)
project(jacktelop)

set(JACKTELOP_VERSION 1.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(USE_GGO OFF)
set(USE_BOOST OFF)

if(APPLE)
  add_definitions(-Wall -Wno-unused-const-variable -g -O3 -Wno-overloaded-virtual)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
   # add the include directories to the build
  set(DIRS gui)
elseif(UNIX)
  add_definitions(-Wall -Wno-unused-but-set-variable -g -O3 -fPIC)
  #add_definitions(-Wall -Wno-unused-but-set-variable -g -O3 )
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x )
  # add the include directories to the build
  set(DIRS listener gui)
endif()


# add the build directory to the include path for generate_lcm() and generate_ggo() outputs
include_directories(${DIRS} ${CMAKE_INSTALL_PREFIX}/include)
include_directories(${CMAKE_BINARY_DIR})

# search for the getgetopt program, exit if not found
if(USE_GGO)
    find_program(GENGETOPT gengetopt)
    if( NOT GENGETOPT )
        message( FATAL_ERROR "gengetopt not found\n" )
        return()
    endif()
endif()

# function to generate the getgetopt source and header from a .ggo file
function(generate_ggo)
  cmake_parse_arguments(GENERATE_GGO "" "INPUT;OUTPUT" "" ${ARGN})
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${GENERATE_GGO_OUTPUT}.c ${CMAKE_BINARY_DIR}/${GENERATE_GGO_OUTPUT}.h
    COMMAND ${GENGETOPT} -i ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATE_GGO_INPUT} --file-name ${GENERATE_GGO_OUTPUT} --output-dir=${CMAKE_BINARY_DIR} --unamed-opts
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATE_GGO_INPUT}
  )
endfunction(generate_ggo)

# search for the lcm package, exit if not found
find_package(lcm)
if( NOT lcm_FOUND )
  message( FATAL_ERROR "lcm not found\n" )
  return()
endif()
include_directories(${LCM_INCLUDE_DIR})

# search for the ROS package if in linux environment
if(APPLE)
  message( "not looking for ROS" )
elseif(UNIX)
  find_package(ROS)
  if( NOT ros_FOUND )
    message( FATAL_ERROR "ROS not found\n" )
    return()
  endif()
  include_directories( ${ROS_INCLUDE_DIR} )
endif()


# function to generate the lcm headers from a .lcm file
function(generate_lcm)
  cmake_parse_arguments(GENERATE_LCM "" "INPUT;OUTPUT" "" ${ARGN})
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${GENERATE_LCM_OUTPUT}
    COMMAND ${LCM_GEN} --python ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATE_LCM_INPUT} --ppath ${CMAKE_BINARY_DIR}/
    COMMAND ${LCM_GEN} --lazy --cpp ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATE_LCM_INPUT} --cpp-hpath ${CMAKE_BINARY_DIR}/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${GENERATE_LCM_INPUT}
  )
endfunction(generate_lcm)

# search for the boost package, exit if not found
if(USE_BOOST)
    find_package(Boost 1.45.0 COMPONENTS system thread filesystem)
    if( NOT Boost_FOUND )
        message( FATAL_ERROR "Boost not found\n" )
        return()
    endif()
    include_directories(${Boost_INCLUDE_DIR})
endif()

if(APPLE)
  # use the OpenGL framework
  set(OPENGL_LIBRARY "-framework GLUT -framework OpenGL")
elseif(UNIX)
  # search for the OpenGL package, exit if not found
  find_package(OpenGL REQUIRED)
  find_package(GLUT REQUIRED)
  if( NOT OPENGL_FOUND )
    message( FATAL_ERROR "OpenGL not found\n" )
    return()
  endif() 
  include_directories(${OPENGL_INCLUDE_DIR})
endif()

# search for the QT package, exit if not found
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
if( NOT Qt4_FOUND )
  message( FATAL_ERROR "Qt4 not found\n" )
  return()
endif()
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})

# function for generate lcm message headers and installing headers
function(process_hdrs)
  cmake_parse_arguments(PROCESS_HDRS "" "" "HDRS;QT_HDRS;LCMS" ${ARGN})
  foreach(lcm ${PROCESS_HDRS_LCMS})
    string(REGEX REPLACE ".lcm" ".hpp" tmp ${lcm})
    string(REGEX REPLACE "msg/" "jacktelop/" tmp ${tmp})
    generate_lcm(INPUT ${lcm} OUTPUT ${tmp})
    set(MSG_HDRS ${MSG_HDRS} ${tmp})
    string(REGEX REPLACE "hpp" "py" tmp ${tmp})
    set(MSG_PY ${MSG_PY} ${tmp})
  endforeach(lcm)
  foreach(msg_hdr ${MSG_HDRS})
    set(msg_hdr_fullpath "${CMAKE_BINARY_DIR}/${msg_hdr}")
    set(MSG_HDRS_FULLPATH ${MSG_HDRS_FULLPATH} ${msg_hdr_fullpath})
  endforeach(msg_hdr)
  if(MSG_PY)
    set(MSG_PY ${MSG_PY} "jacktelop/__init__.py")
  endif()
  foreach(msg_py ${MSG_PY})
    set(msg_py_fullpath "${CMAKE_BINARY_DIR}/${msg_py}")
    set(MSG_PY_FULLPATH ${MSG_PY_FULLPATH} ${msg_py_fullpath})
  endforeach(msg_py)
  install(FILES
          ${HDRS}
          ${QT_HDRS}
          ${MSG_HDRS_FULLPATH}
          DESTINATION include/jacktelop)
  install(FILES
          ${MSG_PY_FULLPATH}
          DESTINATION lib/python2.7/site-packages/jacktelop)
endfunction(process_hdrs)

# function for compiling and installing a library
function(process_lib)
  cmake_parse_arguments(PROCESS_LIB "" "LIB" "SRCS;QT_HDRS;LCMS;DEPS" ${ARGN})
  foreach(lcm ${PROCESS_LIB_LCMS})
    set(tmp "${CMAKE_BINARY_DIR}/${lcm}")
    string(REGEX REPLACE ".lcm" ".hpp" tmp ${tmp})
    string(REGEX REPLACE "msg/" "jacktelop/" tmp ${tmp})
    set(MSG_HDRS ${MSG_HDRS} ${tmp})
  endforeach(lcm)
  if( QT_HDRS )
    qt_wrap_cpp(${LIB} MOC_SRCS ${QT_HDRS})
  endif( QT_HDRS )
  message( "PROCESS_LIB_SRCS=" "${PROCESS_LIB_DEPS}" )
  #add_library(${PROCESS_LIB_LIB} SHARED
  add_library(${PROCESS_LIB_LIB} STATIC
              ${PROCESS_LIB_SRCS}
              ${MOC_SRCS} 
              ${MSG_HDRS})
  target_link_libraries(${PROCESS_LIB_LIB} ${PROCESS_LIB_DEPS})
  install(TARGETS ${PROCESS_LIB_LIB}
        LIBRARY
        DESTINATION lib
        ARCHIVE
        DESTINATION lib)
endfunction(process_lib)

# function for compiling and installing the binaries for the jacktelop project
function(process_bin_srcs)
  cmake_parse_arguments(PROCESS_BIN_SRCS "" "LIB" "BIN_SRCS;GGOS;DEPS" ${ARGN})
  foreach(ggo ${PROCESS_BIN_SRCS_GGOS})
    string(REGEX REPLACE ".ggo" "_cmdline" tmp ${ggo})
    generate_ggo(INPUT ${ggo} OUTPUT ${tmp})
  endforeach(ggo)
  foreach(bin_src ${PROCESS_BIN_SRCS_BIN_SRCS})
      #set(bin_src_cmdline "${CMAKE_BINARY_DIR}/${bin_src}")
      #string(REGEX REPLACE ".cpp" "_cmdline.c" bin_src_cmdline ${bin_src_cmdline})
    set(bin "jacktelop-${bin_src}")
    string(REGEX REPLACE ".cpp" "" bin ${bin})
    string(REGEX REPLACE "_" "-" bin ${bin})
    add_executable(${bin} ${bin_src}) #${bin_src_cmdline})
    message("BIN_SRCS_LIB: " ${PROCESS_BIN_SRCS_LIB} )
    message("BIN_SRCS_DEPS: " ${PROCESS_BIN_SRCS_DEPS} )
    target_link_libraries(${bin} ${PROCESS_BIN_SRCS_DEPS} ${PROCESS_BIN_SRCS_LIB})
    set(BINS ${BINS} ${bin})
  endforeach(bin_src)
  install(TARGETS
          ${BINS}
          DESTINATION bin)
endfunction(process_bin_srcs)

# function to build the documentation for the jacktelop project
if(BUILD_DOCS)
  find_package(Doxygen)
  if( DOXYGEN_FOUND )
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      VERBATIM
    )
    install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/${PROJECT_NAME} DESTINATION doc)
  endif()
endif()

# generates the find_package file for the jacktelop project
configure_file(${PROJECT_SOURCE_DIR}/Find${PROJECT_NAME}.cmake.in ${CMAKE_BINARY_DIR}/Find${PROJECT_NAME}.cmake @ONLY)
install(FILES
        ${CMAKE_BINARY_DIR}/Find${PROJECT_NAME}.cmake
        DESTINATION
        ${CMAKE_INSTALL_PREFIX}/share/cmake-${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}/Modules/)

# add the source directories
foreach(dir ${DIRS})
  add_subdirectory(${dir})
endforeach(dir ${DIRS})

