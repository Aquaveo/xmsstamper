cmake_minimum_required(VERSION 3.1.2)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
set(CMAKE_DEBUG_POSTFIX _d)

if (APPLE OR WIN32)
    set(CMAKE_CXX_STANDARD 17)
else()
    set(CMAKE_CXX_STANDARD 11)
endif()

project(xmsstamperlib C CXX)

if (APPLE)
  set(CMAKE_POSITION_INDEPENDENT_CODE False)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE True)
endif()

set(BUILD_TESTING NO CACHE BOOL "Enable/Disable testing")
set(IS_PYTHON_BUILD NO CACHE BOOL "Set this if you want to build the python bindings.")
set(IS_CONDA_BUILD NO CACHE BOOL "Set this if you want to make a conda package.")
set(PYTHON_TARGET_VERSION 3.6 CACHE STRING "Version of python to link to for python wrapping.")
set(CONDA_PREFIX "" CACHE PATH "Path to the conda environment used to build.")
set(XMS_TEST_PATH ${PROJECT_SOURCE_DIR}/test_files/ CACHE PATH "Path to test files for testing")
set(XMS_VERSION "\"99.99.99\"" CACHE STRING "Library Version")

if(IS_PYTHON_BUILD AND BUILD_TESTING)
  message(FATAL_ERROR "Cannot build python module when testing is enabled")
endif()

add_definitions(-DXMS_VERSION=\"${XMS_VERSION}\")

if(IS_CONDA_BUILD)
  include(${CMAKE_CURRENT_LIST_DIR}/condabuildinfo.cmake)
else() # If we are not using conda, we are using conan
  # Conan setup
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
  set(EXT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS})
  set(EXT_LIB_DIRS ${CONAN_LIB_DIRS})
  set(EXT_LIBS ${CONAN_LIBS})
endif(IS_CONDA_BUILD)

if(WIN32)
    string(COMPARE EQUAL "${CONAN_SETTINGS_COMPILER_RUNTIME}" "MT" USES_MT)
    if(NOT USES_MT)
        string(COMPARE EQUAL "${CONAN_SETTINGS_COMPILER_RUNTIME}" "MTd" USES_MT)
    endif()

    if(USES_MT)
        message("Treating wchar_t as a built-in type.")
        add_definitions(/Zc:wchar_t)  # Treat wchar_t as built-in type
    else()
        message("Treating wchar_t as a typedef.")
        add_definitions(/Zc:wchar_t-)  # Treat wchar_t as typedef
    endif()

    add_definitions(/D BOOST_ALL_NO_LIB)
endif()

message(STATUS "External Include Dirs: ${EXT_INCLUDE_DIRS}")
message(STATUS "External Lib Dirs: ${EXT_LIB_DIRS}")
message(STATUS "Extneral Libs: ${EXT_LIBS}")

if(IS_PYTHON_BUILD)
    # Pybind11 module
    if(IS_CONDA_BUILD)
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Targets.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/FindPythonLibsNew.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Config.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11ConfigVersion.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Tools.cmake")

    else()
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Targets.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/FindPythonLibsNew.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Config.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11ConfigVersion.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Tools.cmake")
    endif()

    list(APPEND EXT_INCLUDE_DIRS
        ${pybind11_INCLUDE_DIR}
    )

    find_package(PythonLibsNew ${PYTHON_TARGET_VERSION} EXACT REQUIRED)

    message("PYTHON_INCLUDE: ${PYTHON_INCLUDE_DIRS}")
    message("PYTHON_LIBS: ${PYTHON_LIBRARIES}")

    list(APPEND EXT_INCLUDE_DIRS
      ${PYTHON_INCLUDE_DIRS}
    )

endif()

message("External Include Dirs: ${EXT_INCLUDE_DIRS}")
message("External Lib Dirs: ${EXT_LIB_DIRS}")
message("Extneral Libs: ${EXT_LIBS}")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIB_DIRS})

# Sources
set(xmsstamper_sources
	xmsstamper/stamper/XmStamper.cpp
	xmsstamper/stamper/XmStamperIo.cpp
	xmsstamper/stamper/TutStamping.cpp
	xmsstamper/stamper/detail/XmBathymetryIntersector.cpp
	xmsstamper/stamper/detail/XmBreaklines.cpp
	xmsstamper/stamper/detail/XmGuideBankUtil.cpp
	xmsstamper/stamper/detail/XmSlopedAbutmentUtil.cpp
	xmsstamper/stamper/detail/XmStampEndCap.cpp
	xmsstamper/stamper/detail/XmStampInterpCrossSection.cpp
	xmsstamper/stamper/detail/XmStampTests.cpp
	xmsstamper/stamper/detail/XmUtil.cpp
)

set(xmsstamper_headers
  xmsstamper/stamper/XmStamper.h
  xmsstamper/stamper/XmStamperIo.h
  xmsstamper/stamper/TutStamping.t.h
  xmsstamper/stamper/detail/XmBathymetryIntersector.h
  xmsstamper/stamper/detail/XmBathymetryIntersector.t.h
  xmsstamper/stamper/detail/XmBreaklines.h
  xmsstamper/stamper/detail/XmGuideBankUtil.h
  xmsstamper/stamper/detail/XmSlopedAbutmentUtil.h
  xmsstamper/stamper/detail/XmStampEndCap.h
  xmsstamper/stamper/detail/XmStamper3dPts.h
  xmsstamper/stamper/detail/XmStampInterpCrossSection.h
  xmsstamper/stamper/detail/XmStampInterpCrossSection.t.h
  xmsstamper/stamper/detail/XmStampTests.t.h
  xmsstamper/stamper/detail/XmUtil.h
)

# Pybind11 sources
set(xmsstamper_py
  xmsstamper/python/xmsstamper_py.cpp
  #Stamper
  xmsstamper/python/stamper/stamper_py.cpp
  xmsstamper/python/stamper/XmStamper_py.cpp
  xmsstamper/python/stamper/XmStamperIo_py.cpp
)

set(xmsstamper_py_headers
  xmsstamper/python/stamper/stamper_py.h
)

# Tests
if (BUILD_TESTING)
 add_definitions(-DXMS_TEST_PATH="${XMS_TEST_PATH}/")
 add_definitions(-DCXX_TEST -DCXXTEST4)

 list(APPEND xmsstamper_sources
  xmsstamper/stamper/detail/XmUtil.t.h
 )

 find_package(CxxTest)
 if(CXXTEST_FOUND)
   include_directories(${CXXTEST_INCLUDE_DIR})
   enable_testing()

   set(CXXTEST_TESTGEN_ARGS --xunit-printer --have-eh)
   file(GLOB_RECURSE test_headers ${CMAKE_CURRENT_LIST_DIR}/xmsstamper/*.t.h)
      CXXTEST_ADD_TEST(
        runner runner.cpp ${test_headers}
      )
      target_link_libraries(runner ${PROJECT_NAME})
 endif()
endif ()

# Static library
add_library(${PROJECT_NAME} STATIC
  ${xmsstamper_sources} ${xmsstamper_headers}
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/>
    ${Boost_INCLUDE_DIR}
 )
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}
  ${EXT_LIBS}
)
target_link_libraries(${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
)
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} rt)
endif()

#Pybind11
if(IS_PYTHON_BUILD)
    pybind11_add_module(_xmsstamper
      ${xmsstamper_py} ${xmsstamper_py_headers}
    )
    target_include_directories(_xmsstamper
      PRIVATE
          ${EXT_LIBS}
          ${PYTHON_INCLUDE_DIRS}
    )
    target_link_libraries(_xmsstamper
    PRIVATE
      ${EXT_LIBS}
      ${PROJECT_NAME}
  )
    set_target_properties(_xmsstamper PROPERTIES
    LINKER_LANGUAGE CXX
  )

  # Install recipe
    install(
      TARGETS _xmsstamper
      ARCHIVE DESTINATION "_package/xms/stamper"
      LIBRARY DESTINATION "_package/xms/stamper"
    )

  install(DIRECTORY "_package/" DESTINATION "_package"
           FILES_MATCHING PATTERN "*.py")

endif()

# Disable Optimization for GCC6 for now... Report issue to gcc and pybind
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND IS_PYTHON_BUILD)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 6 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
    Message("Disabling Optimization for GCC 6...")
    STRING(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    STRING(REPLACE "-O3" "" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  endif()
endif()

# Install recipe
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
)
foreach (header IN LISTS xmsstamper_headers xmsstamper_py_headers)
  get_filename_component(subdir "${header}" DIRECTORY)
  install(
    FILES "${header}"
    DESTINATION "include/${subdir}"
  )
endforeach ()
