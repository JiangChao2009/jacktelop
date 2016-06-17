# LCM FILES
set(LCMS
    msg/Twist_t.lcm 
    msg/Vector3_t.lcm)

# HEADER FILES
set(HDRS
    jacktelop/listener_handler.hpp)

# QT HEADER FILES
set(QT_HDRS )

# SOURCE FILES
set(SRCS
    listener_handler.cpp)

# BINARY SOURCE FILES
set(BIN_SRCS
    listener_demo.cpp)

if( ros_FOUND )
  set( DEPS 
      ${DEPS}
      ${ROS_LIBRARIES} )

endif()

# LIBRARY DEPENDENCIES
set(DEPS ${DEPS} ${LCM_LIBRARIES})

# LIBRARY NAME
set(LIB jacktelop-listener)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS} LCMS ${LCMS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} LCMS ${LCMS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} GGOS ${GGOS} DEPS ${DEPS})
