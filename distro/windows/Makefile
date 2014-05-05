#
# Compile and create OpenTURNS installer for windows
#
# Rules dependencies make things done in the right order, however it does not
# detect if OpenTURNS sources has changed. This has to be specified manually:
# - to recompute a rules, remove the corresponding file
#   i.e. some OpenTURNS .cxx have changed:
#     rm ot-compile; make
# - to recompute every rules: make clean; make
#

###############################################################################
# adapt these following lines to your configuration :

# compiler prefix
TARGET = i686-w64-mingw32
ifeq ($(TARGET),x86_64-w64-mingw32)
  ARCH=x86_64
else
  ARCH=x86
endif

# OpenTurns paths
OT_SRC    ?= $(PWD)/../..
OT_BUILD  ?= $(OT_SRC)/build-$(TARGET)
OT_PREFIX ?= $(OT_BUILD)/install

# openturns-config not present until installed..
#OT_VERSION ?= $(shell $(OT_PREFIX)/bin/openturns-config --version)
OT_VERSION ?= $(shell cat ../../VERSION)

WINDEPS = $(PWD)/openturns-developers-windeps-$(OT_VERSION)



WINE_PREFIX = $(WINDEPS)/wine
export WINEPREFIX = $(WINE_PREFIX)

MINGW_PREFIX = $(WINDEPS)/mingw-w64-x86_64/usr
# OpenTURNS dependencies
PYBASEVER = 27
PYTHON_PREFIX = $(WINE_PREFIX)/drive_c/Python$(PYBASEVER)
R_PATH = $(WINE_PREFIX)/drive_c/R/R-2.12.0
PATH := $(R_PATH)/bin:$(PATH)
CMAKE=cmake

# launch OpenTURNS checktests before creating the installer (y,n)
CHECK_OT=y

# build and install tests
INSTALL_TESTS_OPT=ON

# verbose (ON/OFF)
VERBOSE = ON

JOBS=$(shell getconf _NPROCESSORS_ONLN)
JOBS_PYTHON=$(JOBS)
export PATH := $(MINGW_PREFIX)/bin:$(PATH)

DEBUG_OT=n

###############################################################################
# should not be modified:

export PYTHON=$(PYTHON_PREFIX)/python.exe
export PYTHON_NOVERSIONCHECK=1
export PYTHON_CPPFLAGS=-I$(PYTHON_PREFIX)/include
export PYTHON_LDFLAGS=-L$(PYTHON_PREFIX)/libs -lpython$(PYBASEVER)
export PYTHON_SITE_PKG=$(PYTHON_PREFIX)/Lib/site-packages
# if PYTHON_EXTRA_LIBS is not set to " ", autotools set PYTHON_EXTRA_LIBS to "None" ...
nullstring :=
export PYTHON_EXTRA_LIBS := $(nullstring) # end of the line

BUILD_MACHINE=$(shell gcc -dumpmachine)


###############################################################################
# rules:

.PHONY: all clean-python clean mrproper


all: ot-installer


ot-check-compiler:
	@echo -n "Check OpenTURNS compilation dependencies: " 
	@ls $(OT_SRC) $(PYTHON_PREFIX) $(PYTHON) $(PYTHON_SITE_PKG) $(R_PATH) > /dev/null
	@echo "ok"
	touch ot-check-compiler


ot-config: ot-check-compiler
	-mkdir -p $(OT_BUILD)
	cp $(MINGW_PREFIX)/share/mingw/toolchain-$(TARGET).cmake $(OT_BUILD)
	echo "set ( CMAKE_FIND_ROOT_PATH $(MINGW_PREFIX)/$(TARGET) )" >> $(OT_BUILD)/toolchain-$(TARGET).cmake
	cd $(OT_BUILD) && rm -f CMakeCache.txt && $(CMAKE) \
	  -DCMAKE_TOOLCHAIN_FILE=toolchain-$(TARGET).cmake \
	  -DCMAKE_VERBOSE_MAKEFILE=$(VERBOSE) \
	  -DPYTHON_EXECUTABLE=$(PYTHON_PREFIX)/python.exe \
	  -DPYTHON_INCLUDE_DIR=$(PYTHON_PREFIX)/include \
	  -DPYTHON_INCLUDE_DIR2=$(PYTHON_PREFIX)/include \
	  -DPYTHON_LIBRARY=$(PYTHON_PREFIX)/libs/libpython$(PYBASEVER).a \
	  -DR_EXECUTABLE=$(R_PATH)/bin/R.exe \
	  -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG -g -O2" \
	  -DCMAKE_C_FLAGS_RELEASE="-DNDEBUG -g -O2" \
	  -DCMAKE_Fortran_FLAGS_RELEASE="-DNDEBUG -g -O2" \
	  -DCMAKE_INSTALL_PREFIX=$(OT_PREFIX) \
	  -DINSTALL_TESTS=$(INSTALL_TESTS_OPT) \
	  -DBUILD_PYTHON=ON \
	  $(OT_SRC)
	touch ot-config

ot-compile: ot-config
	cd $(OT_BUILD)/lib && make -j$(JOBS)
	cd $(OT_BUILD) && make -j$(JOBS_PYTHON)
	-beep
	touch ot-compile

ot-install: ot-compile
	cd $(OT_BUILD) && make install -j$(JOBS)
ifeq ($(DEBUG_OT),n)
	$(TARGET)-strip --strip-unneeded $(OT_PREFIX)/bin/*.dll
	$(TARGET)-strip -g $(OT_PREFIX)/lib/*.a
	$(TARGET)-strip --strip-unneeded $(OT_PREFIX)/Lib/site-packages/*/*.pyd
ifeq ($(INSTALL_TESTS_OPT),ON)
	$(TARGET)-strip --strip-all $(OT_PREFIX)/share/openturns/examples/*.exe
endif
endif
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libgcc_s_*-1.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libquadmath-0.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libgfortran-3.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libstdc++-6.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libwinpthread-1.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libblas.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/liblapack.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libxml2-2.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libdl-0.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libgnurx-0.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/libmuparser-2.dll $(OT_PREFIX)/bin
	cp $(MINGW_PREFIX)/$(TARGET)/bin/zlib1.dll $(OT_PREFIX)/bin
	-beep -r 2
	touch ot-install

ot-test: ot-install
	cd $(OT_BUILD) && make tests -j$(JOBS)
ifeq ($(CHECK_OT),y)
	cp $(OT_BUILD)/lib/src/*.dll $(OT_BUILD)/lib/test
	cp $(MINGW_PREFIX)/$(TARGET)/bin/*.dll $(OT_BUILD)/lib/test
	cp $(OT_PREFIX)/bin/libOT*.dll $(OT_BUILD)/python/test
	cp $(MINGW_PREFIX)/$(TARGET)/bin/*.dll $(OT_BUILD)/python/test
	cd $(OT_BUILD) && ctest -j$(JOBS)
	touch ot-test
endif

ot-test-wrapper-template: ot-install
	rm -rf $(OT_BUILD)/mywrapper
# 	svn export $(OT_SRC)/wrappers/WrapperTemplates/wrapper_linked_with_C_function $(OT_BUILD)/mywrapper
	svn export $(OT_SRC)/wrappers/WrapperTemplates/wrapper_linked_with_F77_function $(OT_BUILD)/mywrapper
# 	svn export $(OT_SRC)/wrappers/WrapperTemplates/wrapper_calling_shell_command $(OT_BUILD)/mywrapper
	cd $(OT_BUILD)/mywrapper && ./customize Pouet && mkdir build && cd build && $(CMAKE) \
	  -DCMAKE_TOOLCHAIN_FILE=$(OT_BUILD)/toolchain-$(TARGET).cmake \
	  -DCMAKE_VERBOSE_MAKEFILE=$(VERBOSE) \
	  -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG -g -O2" \
	  -DCMAKE_C_FLAGS_RELEASE="-DNDEBUG -g -O2" \
	  -DCMAKE_Fortran_FLAGS_RELEASE="-DNDEBUG -g -O2" \
	  -DOpenTURNS_DIR=$(OT_PREFIX)/lib/cmake/openturns \
	  -DREGEX_LIBRARIES=$(MINGW_PREFIX)/$(TARGET)/lib/libgnurx.dll.a \
	  -DCMAKE_INSTALL_PREFIX=$(OT_BUILD)/mywrapper/build/install \
	  .. && make install
	cp $(OT_PREFIX)/bin/*.dll $(OT_BUILD)/mywrapper
	cp $(OT_BUILD)/mywrapper/build/install/bin/* $(OT_BUILD)/mywrapper || :
	cd $(OT_BUILD)/mywrapper && PYTHONPATH=$(OT_PREFIX)/Lib/site-packages OPENTURNS_WRAPPER_PATH=$(OT_BUILD)/mywrapper/build/install/wrappers $(PYTHON_PREFIX)/python.exe test.py


ot-check-installer: ot-test
	@echo -n "Check OpenTURNS prefix: "
	@ls $(OT_PREFIX) > /dev/null
	@echo "ok"
	@echo -n "Check nsis compiler: "
	@makensis -VERSION > /dev/null 2>&1
	@echo "ok"
	@echo "OpenTURNS installer dependencies seems all right."
	touch ot-check-installer


ot-user-installer: ot-check-installer
	makensis -DOPENTURNS_PREFIX=$(OT_PREFIX) -DPRODUCT_VERSION=$(OT_VERSION) -DPYBASEVER=$(PYBASEVER) -DARCH=$(ARCH) openturns_script.nsi
	touch ot-user-installer

ot-module-installer: ot-check-installer
	makensis -DOPENTURNS_PREFIX=$(OT_PREFIX) -DPRODUCT_VERSION=$(OT_VERSION) -DPYBASEVER=$(PYBASEVER) -DARCH=$(ARCH) openturns.nsi
	touch ot-module-installer


ot-dev-installer: ot-check-installer
	makensis -DOPENTURNS_PREFIX=$(OT_PREFIX) -DPRODUCT_VERSION=$(OT_VERSION) -DPYBASEVER=$(PYBASEVER) -DARCH=$(ARCH) openturns_developer.nsi
	touch ot-dev-installer


ot-installer: ot-module-installer ot-dev-installer
	#ot-user-installer
	-beep -r 3



clean-python:
	cd $(OT_BUILD)/python && make clean

clean:
	-rm -f ot-*
	-rm -f $(OT_BUILD)/python/src/*_wrap.cxx


mrproper: clean
	-rm -rf $(OT_PREFIX) $(OT_BUILD)
	-rm -f *.exe


