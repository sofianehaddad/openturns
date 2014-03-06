#                                               -*- cmake -*-

# Recurse in subdirectories
add_subdirectory ( include )
add_subdirectory ( src )
add_subdirectory ( etc )

# Build the OpenTURNSConfig.cmake file
set ( OPENTURNS_LIBRARY_DIRS            ${LIBRARY_PATH} )
set ( OPENTURNS_LIBRARY                 OT )
list ( APPEND OPENTURNS_LIBRARIES       ${OPENTURNS_LIBRARY} )
set ( OPENTURNS_WRAPPER_LIBRARIES  ${OPENTURNS_LIBRARIES} )
list ( APPEND OPENTURNS_WRAPPER_LIBRARIES OTbind )
set ( OPENTURNS_INCLUDE_DIR             ${INCLUDE_PATH})
list ( INSERT OPENTURNS_INCLUDE_DIRS 0  ${OPENTURNS_INCLUDE_DIR} )

set ( OPENTURNS_ROOT_DIR       ${INSTALL_PATH} )
set ( OPENTURNS_VERSION_STRING ${PACKAGE_VERSION} )
set ( OPENTURNS_VERSION_MAJOR  ${CPACK_PACKAGE_VERSION_MAJOR} )
set ( OPENTURNS_VERSION_MINOR  ${CPACK_PACKAGE_VERSION_MINOR} )
set ( OPENTURNS_VERSION_PATCH  ${CPACK_PACKAGE_VERSION_PATCH} )
set ( OPENTURNS_USE_FILE       ${CONFIG_CMAKE_PATH}/UseOpenTURNS.cmake )

configure_file ( ${PROJECT_SOURCE_DIR}/lib/OpenTURNSConfig.cmake.in
                 ${PROJECT_BINARY_DIR}/lib/OpenTURNSConfig.cmake
                @ONLY ESCAPE_QUOTES
               )

install ( FILES ${PROJECT_SOURCE_DIR}/lib/UseOpenTURNS.cmake
                ${PROJECT_BINARY_DIR}/lib/OpenTURNSConfig.cmake
          DESTINATION ${OPENTURNS_CONFIG_CMAKE_PATH}
        )

# Recurse in subdirectories : test should be run after src (for target OT to be built)
add_subdirectory ( test )

if ( DOXYGEN_FOUND )

  # generate Doxyfile
  set ( VERSION ${PACKAGE_VERSION} )
  set ( builddir ${CMAKE_CURRENT_BINARY_DIR} )
  set (  top_srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
  configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    @ONLY )
                    
  # generate html doc and compress it
  add_custom_target ( html 
                      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc
                      COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
                      COMMAND tar czf doc-api-${PACKAGE_VERSION}.tar.gz doc
                      COMMAND tar cjf doc-api-${PACKAGE_VERSION}.tar.bz2 doc
                      )                   
                      
endif ( DOXYGEN_FOUND )

install ( FILES ${HEADERFILES}
          DESTINATION ${INCLUDE_PATH}
        )

