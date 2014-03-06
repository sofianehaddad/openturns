#                                               -*- cmake -*-

# workaround for bug #0014358
if ( CMAKE_VERSION VERSION_LESS 2.8.13 AND NOT DEFINED FortranCInterface_EXE )
  set ( FortranCInterface_EXE ${CMAKE_BINARY_DIR}/CMakeFiles/FortranCInterface/FortranCInterface${CMAKE_EXECUTABLE_SUFFIX} )
endif ()

include ( FortranCInterface )
FortranCInterface_HEADER ( FortranCInterface.h )

ot_add_current_dir_to_include_dirs ()
ot_add_build_dir_to_include_dirs ( ${CMAKE_CURRENT_BINARY_DIR} )

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/OTconfig.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/OTconfig.h
                @ONLY
               )

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/installed-OTconfig.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/installed-OTconfig.h
                @ONLY
               )

set ( ot_configure_args )
if ( NOT ot_configure_date )
  execute_process ( COMMAND date -R
                    OUTPUT_VARIABLE ot_configure_date
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                  )
  set ( ot_configure_date "${ot_configure_date}" CACHE STRING "First configure date." )
endif ()
if ( NOT ot_revision )
  execute_process ( COMMAND svnversion
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE ot_revision
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                  )
  set ( ot_revision "${ot_revision}" CACHE STRING "Revision number." )
endif ()
mark_as_advanced( ot_configure_date ot_revision )
set ( prefix ${CMAKE_INSTALL_PREFIX} )
set ( PACKAGE openturns )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/OTconfigure_args.hxx.in
                ${CMAKE_CURRENT_BINARY_DIR}/OTconfigureArgs.hxx
                @ONLY
              )

install ( FILES OTconfig.hxx
                OTdebug.h
                OTmemory.hxx
                OTprivate.hxx
                OTtestcode.hxx
                OTthread.hxx
                OTtypes.hxx
                OTwindows.h
          DESTINATION ${OPENTURNS_INCLUDE_PATH}
        )
install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/installed-OTconfig.h
          DESTINATION ${OPENTURNS_INCLUDE_PATH}
          RENAME OTconfig.h
        )

