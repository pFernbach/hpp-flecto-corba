CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
INCLUDE(cmake/base.cmake)
INCLUDE(cmake/cpack.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)
SET(CXX_DISABLE_WERROR true)

SET(PROJECT_NAME hpp-flecto-corba)
SET(PROJECT_DESCRIPTION
 "Implementation of elastic rod manipulation in hpp, interface with corba"
)
SET(PROJECT_URL "")

SET(${PROJECT_NAME}_HEADERS
   include/hpp/corbaserver/flecto/server.hh
   include/hpp/corbaserver/flecto/fwd.hh
   include/hpp/corbaserver/flecto/node-rod.hh
)
SET(CUSTOM_HEADER_DIR hpp/corbaserver/flecto)

SETUP_PROJECT()

# Activate hpp-util logging if requested
SET (HPP_DEBUG FALSE CACHE BOOL "trigger hpp-util debug output")
IF (HPP_DEBUG)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHPP_DEBUG")
ENDIF()

ADD_REQUIRED_DEPENDENCY("gepetto-viewer")
ADD_REQUIRED_DEPENDENCY("hpp-corbaserver >= 3")
#ADD_REQUIRED_DEPENDENCY("hpp-flecto >= 3") A enlever apres
ADD_REQUIRED_DEPENDENCY("hpp-template-corba >= 1.1")
ADD_REQUIRED_DEPENDENCY("hpp-util >= 3")

# This dependency ensures us that the intermediate __init__.py files are
# already installed.
ADD_REQUIRED_DEPENDENCY("hpp-corbaserver >= 2.1")
# Make sure that python submodule is installed in the same prefix as module.
IF (NOT ${HPP_CORBASERVER_PREFIX} STREQUAL ${CMAKE_INSTALL_PREFIX})
  MESSAGE(WARNING
    "  THIS PACKAGE SHOULD BE INSTALLED IN THE SAME PREFIX AS HPP-CORBASERVER,\n"
    "  SINCE IT INSTALLS A PYTHON SUBMODULE OF HPP.CORBASERVER.")
  MESSAGE("CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
  MESSAGE("HPP_CORBASERVER_PREFIX=${HPP_CORBASERVER_PREFIX}")
ENDIF()

PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

ADD_SUBDIRECTORY(src)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()






