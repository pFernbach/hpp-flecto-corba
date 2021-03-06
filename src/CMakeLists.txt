SET(IDL_SOURCES
  #mettre le nom du fichier dans idl/hpp/corbaserver sans le .py
  Rod
  )

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)
FOREACH(IDL ${IDL_SOURCES})
  GENERATE_IDL_FILE (${IDL}
    ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/flecto)
ENDFOREACH()

SET(LIBRARY_NAME ${PROJECT_NAME})
ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  server.cc
  rod.impl.hh
  rod.impl.cc
  RodSK.cc
  node-rod.cc
  ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/flecto/Rod_idl.py
)

#PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-flecto)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-template-corba)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-util)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} gepetto-viewer)

INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})

# search for python
FINDPYTHON()

# search for omniidl
FIND_PROGRAM(OMNIIDL omniidl)
IF(${OMNIIDL} STREQUAL OMNIIDL-NOTFOUND)
  MESSAGE(FATAL_ERROR "cannot find omniidl.")
ENDIF(${OMNIIDL} STREQUAL OMNIIDL-NOTFOUND)

FOREACH (IDL ${IDL_SOURCES})
  ADD_CUSTOM_COMMAND(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/flecto/${IDL}_idl.py
    COMMAND ${OMNIIDL} ARGS -bpython -Wbpackage=""
    ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/flecto/${IDL}.idl
    MAIN_DEPENDENCY
    ${CMAKE_SOURCE_DIR}/idl/hpp/corbaserver/flecto/${IDL}.idl
    )
  SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_CURRENT_BINARY_DIR}/hpp/corbaserver/flecto/${IDL}_idl.py
    GENERATED=ON
    )
  INSTALL(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL}_idl.py
    DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver/flecto
    )
ENDFOREACH()
INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/hpp/corbaserver/flecto/__init__.py
  DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver/flecto
  )
  
  install (FILES
  hpp/corbaserver/rod/robot_rod.py
  hpp/corbaserver/rod/__init__.py
  DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver/rod)
  
  
INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/hpp/corbaserver/flecto/client.py
  ${CMAKE_CURRENT_SOURCE_DIR}/hpp/corbaserver/flecto/flectoViewer.py
  DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver/flecto
  )
#install(FILES
#  hpp/corbaserver/flecto.py
#  hpp/corbaserver/__init__.py
#  DESTINATION ${PYTHON_SITELIB}/hpp/corbaserver
#  )
# Standalone corba server
ADD_EXECUTABLE (hpp-flecto-server hpp-flecto-corba.cc)
TARGET_LINK_LIBRARIES (hpp-flecto-server ${LIBRARY_NAME} )
TARGET_LINK_LIBRARIES (hpp-flecto-server ${QSERL_LIBRARY})
#PKG_CONFIG_USE_DEPENDENCY(hpp-flecto-server hpp-flecto)
PKG_CONFIG_USE_DEPENDENCY(hpp-flecto-server hpp-corbaserver)
PKG_CONFIG_USE_DEPENDENCY(hpp-flecto-server gepetto-viewer)
PKG_CONFIG_USE_DEPENDENCY(hpp-flecto-server hpp-template-corba)
PKG_CONFIG_USE_DEPENDENCY(hpp-flecto-server hpp-util)

INSTALL (TARGETS hpp-flecto-server DESTINATION ${CMAKE_INSTALL_BINDIR})
