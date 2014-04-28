#                                               -*- cmake -*-

cmake_minimum_required ( VERSION 2.8 )

option ( USE_BISON                    "Looks for Bison if true and then build parser"                         ON )
option ( USE_FLEX                     "Looks for Flex if true and then build lexer"                           ON )
option ( USE_TBB                      "Use Intel Threading Building Blocks library for multithreading"        ON )
option ( USE_SYSTEM_LAPACK            "Use system LAPACK libraries"                                           ON )
option ( USE_MUPARSER                 "Use muParser libraries"                                                ON )
option ( USE_MUPARSER2                "Use muParser2 features"                                                OFF )
option ( USE_LIBXML2                  "Use LibXml2 for XML support"                                           ON )
option ( USE_R                        "Use R for graph output support"                                        ON )
option ( USE_OPENMP                   "Use OpenMP for multithreading"                                         OFF )
option ( USE_BOOST                    "Use Boost for distribution computation"                                ON )
option ( SYSTEM_INSTALL               "True if Open TURNS is installed in a system directory. RPATH removed." OFF )
option ( BUILD_LIB                    "Build the library"                                                     ON )
option ( BUILD_PYTHON                 "Build the python module for the library"                               ON )
option ( BUILD_VALIDATION             "Build the validation files of the library"                             ON )
option ( LINK_PYTHON_LIBRARY          "Link python modules against python library"                            ON )
option ( INSTALL_TESTS                "Install compiled tests for native platform testing"                    OFF )
option ( BUILD_SHARED_LIBS            "build shared libraries"                                                ON )

# By default, build in Release mode. Must appear before project() command
if ( NOT DEFINED CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "Build type" )
endif ()

project ( OpenTURNS CXX )

# Defines our own module path
list ( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )


set ( OPENTURNS_HOME_ENV_VAR OPENTURNS_HOME     )

if ( NOT DEFINED CMAKE_INSTALL_LIBDIR )
  set ( CMAKE_INSTALL_LIBDIR lib${LIB_SUFFIX} )
endif ()

set ( OPENTURNS_LIBRARY_PATH
      ${CMAKE_INSTALL_LIBDIR}
      CACHE PATH "The directory where the library is installed"
    )
set ( OPENTURNS_INCLUDE_PATH
      include/openturns
      CACHE PATH "The directory where the header files are installed"
    )
set ( OPENTURNS_CONFIG_CMAKE_PATH
      ${CMAKE_INSTALL_LIBDIR}/cmake/openturns
      CACHE PATH "The directory where the CMake files are installed"
    )
set ( OPENTURNS_SYSCONFIG_PATH
      etc
      CACHE PATH "The directory where the configuration file is installed"
    )
set ( OPENTURNS_DATA_PATH
      share
      CACHE PATH "The directory where the common files are installed"
    )
set ( OPENTURNS_WRAPPER_PATH
      ${CMAKE_INSTALL_LIBDIR}/openturns/wrappers
      CACHE PATH "The directory where the wrapper files are installed"
    )
set ( OPENTURNS_EXAMPLE_PATH
      share/openturns/examples
      CACHE PATH "The directory where the example files are installed"
    )
set ( OPENTURNS_SAMPLE_PATH
      share/openturns/examples
      CACHE PATH "The directory where the sample files are installed"
    )
set ( OPENTURNS_VALIDATION_PATH
      share/openturns/validation
      CACHE PATH "The directory where validation files are installed"
    )
set ( OPENTURNS_DOC_PATH
      share/openturns/doc
      CACHE PATH "The directory where the license files are installed"
    )

if ( WIN32 )
  set ( DEFAULT_TMP TEMP )
else ( WIN32 )
  set( DEFAULT_TMP /tmp )
endif( WIN32 )

set ( OPENTURNS_TEMPDIR
      ${DEFAULT_TMP}
      CACHE PATH "The directory for temporary files. /tmp by default."
    )
set ( TEMPDIR ${OPENTURNS_TEMPDIR} )

# WARNING: This is where variables without OPENTURNS_ prefix are defined !
foreach ( _var INSTALL_PATH LIBRARY_PATH INCLUDE_PATH CONFIG_CMAKE_PATH SYSCONFIG_PATH DATA_PATH WRAPPER_PATH EXAMPLE_PATH SAMPLE_PATH VALIDATION_PATH DOC_PATH )
  if ( IS_ABSOLUTE ${OPENTURNS_${_var}} )
    set(${_var} ${OPENTURNS_${_var}})
  else ( IS_ABSOLUTE ${OPENTURNS_${_var}} )
    set(${_var} ${CMAKE_INSTALL_PREFIX}/${OPENTURNS_${_var}})
  endif ( IS_ABSOLUTE ${OPENTURNS_${_var}} )
endforeach ( _var )

# CMake 2.x does not define MSVC when using Intel compiler on Windows, see
# http://public.kitware.com/Bug/view.php?id=14476
if ( WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Intel" )
  set(WINTEL TRUE)
endif()


if ( BUILD_LIB )

enable_language ( C )

if ( MSVC OR WINTEL )
  set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/lib/include/pthread_win32 ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
  include_directories(${PROJECT_SOURCE_DIR}/lib/include/pthread_win32 ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
  find_package(Boost REQUIRED)
  set(USE_BOOST ON CACHE INTERNAL "Force use of Boost on Windows")
endif ()

if ( USE_BISON )
  find_package ( BISON 2.4 )
  if ( BISON_FOUND )
    set ( HAVE_BISON TRUE )
  endif ()
endif ()

if ( USE_FLEX )
  find_package ( FLEX )
  if ( FLEX_FOUND )
    set ( HAVE_FLEX TRUE )
  endif ()
endif ()

find_program ( BC_EXECUTABLE NAMES bc )

if ( USE_TBB )
  find_package ( TBB )
  if ( TBB_FOUND AND (MSVC OR WINTEL ))
    set (OPENTURNS_TBB_NO_IMPLICIT_LINKAGE ON CACHE BOOL "Prevent implicit linkage against tbb.lib ")
  endif ()
endif ()

if ( USE_SYSTEM_LAPACK )
  find_package ( LAPACKE NO_MODULE )
  if ( NOT LAPACKE_FOUND )
    enable_language( Fortran )
    find_package ( LAPACK REQUIRED )
  endif ()
endif ()
if ( NOT LAPACK_FOUND AND NOT LAPACKE_FOUND )
  include ( ExternalProject )
  enable_language( Fortran )
  set ( LAPACK_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lapack-prefix )
  ExternalProject_Add ( lapack
    URL http://www.netlib.org/lapack/lapack-3.5.0.tgz
    URL_MD5 b1d3e3e425b2e44a06760ff173104bdf
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${LAPACK_PREFIX}
      -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
      -DBUILD_TESTING=OFF
      "-DCMAKE_Fortran_FLAGS=-g -O2 -fPIC"
  )
  list ( APPEND OPENTURNS_PRIVATE_LIBRARIES ${LAPACK_PREFIX}/lib/libblas.a )
  list ( APPEND OPENTURNS_PRIVATE_LIBRARIES ${LAPACK_PREFIX}/lib/liblapack.a )
endif ()

if ( USE_MUPARSER )
  find_package ( muParser 1.32 )
  if ( MUPARSER_FOUND )
    set ( OPENTURNS_HAVE_MUPARSER TRUE )
    if ( USE_MUPARSER2 AND NOT MUPARSER_VERSION_STRING VERSION_LESS 2 )
      set ( OPENTURNS_HAVE_MUPARSER2 TRUE )
    endif ()
  endif ()
endif ()
  
find_package ( Doxygen )

if ( USE_LIBXML2 )
  find_package ( LibXml2 )
endif ()

find_package ( Threads )
find_package ( Libdl )
find_package ( Regex )

if ( USE_BOOST )
  # doesn't pass all tests with boost <=1.42 (debian6)
  find_package ( Boost 1.46 )
endif ()

if ( USE_R )
  find_package ( R COMPONENTS base rot )
endif ()

if ( USE_OPENMP )
  find_package ( OpenMP )
endif ()
if ( OPENMP_FOUND )
  set ( CMAKE_CXX_FLAGS "${OpenMP_CXX_FLAGS} ${CMAKE_CXX_FLAGS}" )
endif ()

include ( TestBigEndian )
test_big_endian ( __BIG_ENDIAN__ ) 

try_compile(OPENTURNS_UNSIGNEDLONG_SAME_AS_UINT64
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/unsignedlong_same_as_uint64_t.cxx )

# Find system prerequisites
include ( CheckIncludeFile )
check_include_file ( arpa/inet.h   OPENTURNS_HAVE_ARPA_INET_H   )
check_include_file ( assert.h      OPENTURNS_HAVE_ASSERT_H      )
check_include_file ( dirent.h      OPENTURNS_HAVE_DIRENT_H      )
check_include_file ( errno.h       OPENTURNS_HAVE_ERRNO_H       )
check_include_file ( inttypes.h    OPENTURNS_HAVE_INTTYPES_H    )
check_include_file ( malloc.h      OPENTURNS_HAVE_MALLOC_H      )
check_include_file ( math.h        OPENTURNS_HAVE_MATH_H        )
check_include_file ( netdb.h       OPENTURNS_HAVE_NETDB_H       )
check_include_file ( netinet/in.h  OPENTURNS_HAVE_NETINET_IN_H  )
check_include_file ( netinet/tcp.h OPENTURNS_HAVE_NETINET_TCP_H )
check_include_file ( pthread.h     OPENTURNS_HAVE_PTHREAD_H     )
check_include_file ( regex.h       OPENTURNS_HAVE_REGEX_H       )
check_include_file ( semaphore.h   OPENTURNS_HAVE_SEMAPHORE_H   )
check_include_file ( signal.h      OPENTURNS_HAVE_SIGNAL_H      )
check_include_file ( stdbool.h     OPENTURNS_HAVE_STDBOOL_H     )
check_include_file ( stdint.h      OPENTURNS_HAVE_STDINT_H      )
check_include_file ( stdlib.h      OPENTURNS_HAVE_STDLIB_H      )
check_include_file ( string.h      OPENTURNS_HAVE_STRING_H      )
check_include_file ( strings.h     OPENTURNS_HAVE_STRINGS_H     )
check_include_file ( sys/socket.h  OPENTURNS_HAVE_SYS_SOCKET_H  )
check_include_file ( sys/stat.h    OPENTURNS_HAVE_SYS_STAT_H    )
check_include_file ( sys/time.h    OPENTURNS_HAVE_SYS_TIME_H    )
check_include_file ( sys/types.h   OPENTURNS_HAVE_SYS_TYPES_H   )
check_include_file ( sys/un.h      OPENTURNS_HAVE_SYS_UN_H      )
check_include_file ( unistd.h      OPENTURNS_HAVE_UNISTD_H      )

else ()
  find_package ( OpenTURNS NO_MODULE REQUIRED )
endif ()

# Some useful macros to ease CMakeLists.txt file writing
set ( SOURCEFILES "" CACHE INTERNAL "List of source files to compile" )
macro ( ot_add_source_file FILENAME )
  set ( sf ${SOURCEFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( SOURCEFILES ${sf} CACHE INTERNAL "List of source files to compile" )
endmacro ( ot_add_source_file )

set ( BUILT_SOURCEFILES "" CACHE INTERNAL "List of source files to compile" )
macro ( ot_add_built_source_file FILENAME )
  set ( sf ${BUILT_SOURCEFILES} ${FILENAME} )
  set ( BUILT_SOURCEFILES ${sf} CACHE INTERNAL "List of built source files to compile" )
endmacro ( ot_add_built_source_file )

set ( HEADERFILES "" CACHE INTERNAL "List of header files to install" )
macro ( ot_install_header_file FILENAME )
  set ( hf ${HEADERFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( HEADERFILES ${hf} CACHE INTERNAL "List of header files to install" )
endmacro ( ot_install_header_file )

set ( SWIGFILES "" CACHE INTERNAL "List of SWIG files to install" )
macro ( ot_install_swig_file FILENAME )
  set ( hf ${SWIGFILES} ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME} )
  set ( SWIGFILES ${hf} CACHE INTERNAL "List of SWIG files to install" )
endmacro ( ot_install_swig_file )

set ( INTERNAL_INCLUDE_DIRS "" CACHE INTERNAL "List of directories with header files needed for build" )
macro ( ot_add_current_dir_to_include_dirs )
  set ( inc_dirs ${INTERNAL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )
  set ( INTERNAL_INCLUDE_DIRS ${inc_dirs} CACHE INTERNAL "List of directories with header files needed for build" )
endmacro ( ot_add_current_dir_to_include_dirs )
macro ( ot_add_build_dir_to_include_dirs DIR )
  set ( inc_dirs ${INTERNAL_INCLUDE_DIRS} ${DIR} )
  set ( INTERNAL_INCLUDE_DIRS ${inc_dirs} CACHE INTERNAL "List of directories with header files needed for build" )
endmacro ( ot_add_build_dir_to_include_dirs )

if ( WIN32 )
  list ( APPEND OPENTURNS_LIBRARIES ws2_32 )
endif ()

# Add definitions according to the prerequisites found
add_definitions( -DHAVE_CONFIG_H )

if (NOT BUILD_SHARED_LIBS)
  list ( APPEND OPENTURNS_DEFINITIONS -DOT_STATIC )
endif ()

# CMake makefiles don't link against stdc++ on OSX
if ( APPLE )
  list ( APPEND OPENTURNS_LIBRARIES stdc++ )
endif ()

if ( MSVC OR WINTEL )
  list ( APPEND OPENTURNS_DEFINITIONS -D_USE_MATH_DEFINES )
endif ()

if ( LIBXML2_FOUND )
  set ( HAVE_LIBXML2 TRUE )
  set ( OPENTURNS_HAVE_LIBXML2 TRUE )
  list ( APPEND OPENTURNS_DEFINITIONS ${LIBXML2_DEFINITIONS} )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )
  list ( APPEND OPENTURNS_LIBRARIES ${LIBXML2_LIBRARIES} )
endif ()

if ( LAPACK_FOUND )
  list ( APPEND OPENTURNS_LIBRARIES ${LAPACK_LIBRARIES} )
endif ()
if ( LAPACKE_FOUND )
  set ( OPENTURNS_HAVE_LAPACKE TRUE )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${LAPACKE_INCLUDE_DIRS} )
  list ( APPEND OPENTURNS_LIBRARIES ${LAPACKE_LIBRARIES} )
endif ()

if ( MUPARSER_FOUND )
  list ( APPEND OPENTURNS_LIBRARIES ${MUPARSER_LIBRARIES} )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${MUPARSER_INCLUDE_DIRS} )
endif ()

if ( Threads_FOUND )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${THREADS_INCLUDE_DIRS} )
  list ( APPEND OPENTURNS_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} )
endif ()

if ( TBB_FOUND )
  set ( HAVE_TBB TRUE )
  set ( HAVE_TBB_TBB_H TRUE )
  set ( OPENTURNS_HAVE_TBB TRUE )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${TBB_INCLUDE_DIRS} )
  list ( APPEND OPENTURNS_LIBRARIES ${TBB_LIBRARIES} )
endif ()

if ( LIBDL_FOUND )
  set ( OPENTURNS_HAVE_LIBDL TRUE )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${LIBDL_INCLUDE_DIR} )
  list ( APPEND OPENTURNS_LIBRARIES ${LIBDL_LIBRARIES} )
endif ()

if ( REGEX_FOUND )
  set ( HAVE_REGEX TRUE )
  list ( APPEND OPENTURNS_INCLUDE_DIRS ${REGEX_INCLUDE_DIR} )
  list ( APPEND OPENTURNS_LIBRARIES ${REGEX_LIBRARIES} )
endif ()

if ( OPENMP_FOUND ) 
  list ( APPEND OPENTURNS_LIBRARIES ${OpenMP_CXX_FLAGS} )
endif ()

if ( Boost_FOUND )
  set ( HAVE_BOOST TRUE )
  list ( APPEND OPENTURNS_PRIVATE_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} )
endif ()

add_definitions ( ${OPENTURNS_DEFINITIONS} )
if ( OPENTURNS_INCLUDE_DIRS )
  list ( REMOVE_DUPLICATES OPENTURNS_INCLUDE_DIRS )
endif ( OPENTURNS_INCLUDE_DIRS )
include_directories ( ${OPENTURNS_INCLUDE_DIRS} )
include_directories ( ${OPENTURNS_PRIVATE_INCLUDE_DIRS} )

set ( CPACK_PACKAGE_NAME          openturns      )
set ( CPACK_PACKAGE_VERSION_MAJOR 1              )
set ( CPACK_PACKAGE_VERSION_MINOR 3 )
set ( CPACK_PACKAGE_VERSION_PATCH                )
set ( CPACK_SOURCE_GENERATOR      "TGZ;TBZ2"     )
set ( CPACK_BINARY_STGZ           "OFF"          )
set ( CPACK_BINARY_TBZ2           "ON"           )
set ( CPACK_BINARY_TGZ            "ON"           )
set ( CPACK_BINARY_TZ             "OFF"          )
set ( CPACK_SOURCE_IGNORE_FILES "/.svn;/build;.*~;${CPACK_SOURCE_IGNORE_FILES}" )
#set ( CPACK_INSTALL_CMAKE_PROJECTS "lib;OpenTURNSLib;ALL;/" "python;OpenTURNSPython;ALL;/" )


# Set global definitions
# TODO: remove PACKAGE_<XX> from source code. Needed for compatibility with autotools
set ( PACKAGE_NAME           ${CPACK_PACKAGE_NAME} )
set ( PACKAGE_VERSION        ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR} )
if ( CPACK_PACKAGE_VERSION_PATCH )
  set ( PACKAGE_VERSION       ${PACKAGE_VERSION}.${CPACK_PACKAGE_VERSION_PATCH} )
endif ( CPACK_PACKAGE_VERSION_PATCH )
set ( PACKAGE_BUGREPORT      bugs@openturns.org )
set( CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION} )

# uninstall target
configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
  IMMEDIATE @ONLY
)

add_custom_target ( uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

# distcheck target
find_program( TAR_PROGRAM tar ) 
get_filename_component( BUILD_TOOL_NAME ${CMAKE_BUILD_TOOL} NAME )
if ( TAR_PROGRAM AND BUILD_TOOL_NAME MATCHES ".*make" )
  set ( DISTCHECK_BINARY_DIR ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}/_build )
  set ( DISTCHECK_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}/_inst )
  add_custom_target ( distcheck 
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} "$(MAKE)" package_source
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DISTCHECK_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DISTCHECK_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ${TAR_PROGRAM} xvfz ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DISTCHECK_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DISTCHECK_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} ${CMAKE_COMMAND} .. -DCMAKE_INSTALL_PREFIX=${DISTCHECK_INSTALL_PREFIX}
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)"
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" check
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" install
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" cppinstallcheck
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" pyinstallcheck
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" uninstall
    COMMAND ${CMAKE_COMMAND} -E chdir ${DISTCHECK_BINARY_DIR} "$(MAKE)" clean
  )
endif ()

# Selectively add or remove RPATH from executable
if ( NOT SYSTEM_INSTALL )
  set ( CMAKE_INSTALL_RPATH ${LIBRARY_PATH}
                            ${CMAKE_Fortran_IMPLICIT_LINK_DIRECTORIES}
      )
  set ( CMAKE_BUILD_WITH_INSTALL_RPATH    TRUE )
  set ( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
endif ()

set ( OPENTURNS_SWIG_INCLUDE_DIRS ${INCLUDE_PATH}/swig )
set ( OPENTURNS_SWIG_DEFINITIONS -DSWIG_TYPE_TABLE=openturns )

# The tests can't be run if this function is absent
enable_testing ()
add_custom_target ( tests COMMENT "Build tests" )
add_custom_target ( check COMMENT "Run pre-installation tests" )
add_custom_target ( installcheck COMMENT "Run post-installation tests" )

# Recurse in subdirectories
add_subdirectory ( wrappers )

if ( BUILD_PYTHON )
  # workaround for cmake bug #0013449
  if ( NOT DEFINED CMAKE_FIND_ROOT_PATH )
    find_package ( SWIG )
  else ()
    find_program ( SWIG_EXECUTABLE NAMES swig2.0 swig )
    if ( SWIG_EXECUTABLE )
      set ( SWIG_USE_FILE ${CMAKE_ROOT}/Modules/UseSWIG.cmake )
      set ( SWIG_FOUND TRUE )
    endif ()
  endif ()
  if ( SWIG_FOUND )
    include ( ${SWIG_USE_FILE} )
    mark_as_advanced ( SWIG_VERSION SWIG_DIR )
  endif ()
  find_package ( PythonInterp )
  find_package ( PythonLibs )
  include ( FindPythonModule )
  find_python_module ( numpy )
  find_python_module ( scipy )
  find_python_module ( matplotlib 1.1 )

  if ( PYTHONINTERP_FOUND )
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print( sysconfig.get_python_lib( plat_specific=True, prefix='${CMAKE_INSTALL_PREFIX}' ) )"
                      OUTPUT_VARIABLE _ABS_PYTHON_MODULE_PATH
                      RESULT_VARIABLE _PYTHON_pythonlib_result
                      OUTPUT_STRIP_TRAILING_WHITESPACE )
    
    if (NOT _PYTHON_pythonlib_result)
      get_filename_component ( _ABS_PYTHON_MODULE_PATH ${_ABS_PYTHON_MODULE_PATH} ABSOLUTE )
      file ( RELATIVE_PATH _REL_PYTHON_MODULE_PATH ${CMAKE_INSTALL_PREFIX} ${_ABS_PYTHON_MODULE_PATH} )                   
                          
      set ( OPENTURNS_PYTHON_MODULE_PATH
            ${_REL_PYTHON_MODULE_PATH}
            CACHE PATH "The directory where the python modules are installed"
          )

      if ( IS_ABSOLUTE ${OPENTURNS_PYTHON_MODULE_PATH} )
        set( PYTHON_MODULE_PATH ${OPENTURNS_PYTHON_MODULE_PATH} )
      else ()
        set( PYTHON_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/${OPENTURNS_PYTHON_MODULE_PATH} )
      endif ()
      
    else ()
      message ( SEND_ERROR "Could not run ${PYTHON_EXECUTABLE}" )
    endif ()
  endif ()
endif ()


if ( BUILD_LIB )
  add_subdirectory ( lib )
  if ( NOT LAPACK_FOUND )
    add_dependencies ( OT lapack )
  endif ()
  if ( NOT MUPARSER_FOUND )
    add_dependencies ( OT muparser )
  endif ()
  add_dependencies ( check cppcheck )
  add_dependencies ( installcheck cppinstallcheck )
endif ( BUILD_LIB )

if ( PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND AND SWIG_FOUND )
  add_subdirectory ( python )
  add_dependencies ( installcheck pyinstallcheck )
endif ()


if ( BUILD_VALIDATION )
  add_subdirectory ( validation )
endif ()


include ( CPack )

install ( FILES COPYING
                COPYING.LESSER
                COPYING.cobyla
                COPYING.dsfmt
                COPYING.ev3
                COPYING.faddeeva
                COPYING.kendall
                COPYING.kissfft
                COPYING.KolmogorovSmirnovDist
                COPYING.poissinv
                COPYING.tnc
          DESTINATION ${OPENTURNS_DOC_PATH}
        )
