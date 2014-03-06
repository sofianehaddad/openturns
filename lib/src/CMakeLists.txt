#                                               -*- cmake -*-

# Register current directory files
ot_add_current_dir_to_include_dirs ()
ot_install_header_file ( OT.hxx )
ot_add_source_file ( openturns_library_ok.c )

foreach ( include_dir ${OPENTURNS_INCLUDE_DIRS} )
  set ( EXTRA_CPPFLAGS "${EXTRA_CPPFLAGS} -I${include_dir}" )
endforeach ()
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/openturns-config-build.in
                 ${CMAKE_CURRENT_BINARY_DIR}/openturns-config-build
                 @ONLY
               )
set (WGET wget)
set (CURL curl)
set (prefix "${CMAKE_INSTALL_PREFIX}")
set (exec_prefix "${CMAKE_INSTALL_PREFIX}")
set (libdir "${INSTALL_PATH}/lib${LIB_SUFFIX}")
set (PACKAGE "${CPACK_PACKAGE_NAME}")
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/openturns-module-build.in
                 ${CMAKE_CURRENT_BINARY_DIR}/openturns-module-build
                 @ONLY
               )
install ( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/openturns-config-build
          DESTINATION ${INSTALL_PATH}/bin
          RENAME openturns-config
        )
install ( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/openturns-module-build
          DESTINATION ${INSTALL_PATH}/bin
          RENAME openturns-module
        )

# Recurse in subdirectories
add_subdirectory ( Base )
add_subdirectory ( Uncertainty )

# Build main library
foreach ( built_source ${BUILT_SOURCEFILES} )
  set_source_files_properties ( ${built_source} PROPERTIES GENERATED TRUE )
endforeach ( built_source )
include_directories ( ${INTERNAL_INCLUDE_DIRS} )
link_directories ( ${CMAKE_Fortran_IMPLICIT_LINK_DIRECTORIES} )
add_library ( OTbind bind.c )
add_library ( OT ${SOURCEFILES} ${BUILT_SOURCEFILES} )

# soversion
if ( NOT DEFINED LIB_VERSION ) 
  set ( LIB_VERSION 0.0.0 )
endif ()
if ( NOT DEFINED LIB_SOVERSION ) 
  set ( LIB_SOVERSION 0 )
endif ()
set_target_properties ( OT OTbind PROPERTIES SOVERSION ${LIB_SOVERSION} )
set_target_properties ( OT OTbind PROPERTIES VERSION ${LIB_VERSION} )

if ( BISON_FOUND )
  add_dependencies ( OT generate_csv_parser )
endif ( BISON_FOUND )
if ( FLEX_FOUND )
  add_dependencies ( OT generate_csv_lexer  )
endif ( FLEX_FOUND )

# CMake makefiles don't link against stdc++ on OSX
if ( APPLE )
  target_link_libraries ( OTbind stdc++ )
endif ( APPLE )

target_link_libraries ( OT ${OPENTURNS_LIBRARIES} )
target_link_libraries ( OT ${OPENTURNS_PRIVATE_LIBRARIES} )

# Build an empty executable to check link dependencies and completeness
add_executable ( linktest main.cxx )
target_link_libraries ( linktest OT )
set_target_properties ( linktest PROPERTIES
                                 INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${CMAKE_BINARY_DIR}/lib/src" )


# Install rules for library and executable
install ( TARGETS OT OTbind
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION ${LIBRARY_PATH}
          ARCHIVE DESTINATION ${LIBRARY_PATH}
        )

#foreach ( file ${SOURCEFILES} )
#  message ( ">> Will compile ${file}" )
#endforeach ( file )

#foreach ( file ${BUILT_SOURCEFILES} )
#  message ( ">> Will compile built ${file}" )
#endforeach ( file )

#foreach ( file ${HEADERFILES} )
#  message ( ">> Will install ${file}" )
#endforeach ( file )

#get_cmake_property ( VARS VARIABLES )
#foreach ( var ${VARS} )
#  file( APPEND ${CMAKE_CURRENT_BINARY_DIR}/VARS.txt
#        "${var} \"${${var}}\"\n" )
#endforeach ( var )
