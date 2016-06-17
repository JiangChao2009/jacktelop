# LCM FILES
set(LCMS 
    msg/Twist_t.lcm
    msg/Vector3_t.lcm)

# HEADER FILES
set(HDRS )

# QT HEADER FILES
set(QT_HDRS 
    jacktelop/keywindow.hpp )


# SOURCE FILES
set(SRCS 
    gui.cpp)

# BINARY SOURCE FILES
set(BIN_SRCS
    gui_demo.cpp )

# LIBRARY DEPENDENCIES
set(DEPS ${LCM_LIBRARIES} ${QT_LIBRARIES})

# LIBRARY NAME
set(LIB jacktelop-gui)

# process the headers
process_hdrs(HDRS ${HDRS} QT_HDRS ${QT_HDRS} LCMS ${LCMS})

# process the library
process_lib(LIB ${LIB} QT_HDRS ${QT_HDRS} SRCS ${SRCS} LCMS ${LCMS} DEPS ${DEPS})

# process the binaries
process_bin_srcs(LIB ${LIB} BIN_SRCS ${BIN_SRCS} DEPS ${DEPS})
