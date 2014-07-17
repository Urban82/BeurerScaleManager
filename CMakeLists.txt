project(BeurerScaleManager)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

# Debug
set(DEBUG ON)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    set(DEBUG OFF)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")

include(MacroListAddPrefix)

include(Dependencies)

include_directories(${QT_INCLUDES} ${LIBUSB_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(BSM_SRCS src/main.cpp src/utils.cpp)
set(BSM_HDRS)
set(BSM_UIS)
set(BSM_RCS)
set(BSM_QMS)

add_subdirectory(src)
add_subdirectory(resources)
add_subdirectory(translations)

qt4_wrap_cpp(BSM_SRCS ${BSM_HDRS})
qt4_wrap_ui(BSM_SRCS ${BSM_UIS})
qt4_add_resources(BSM_SRCS ${BSM_RCS})

set_source_files_properties(${BSM_QMS} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/translations")
qt4_add_translation(BSM_SRCS ${BSM_QMS})

if(NOT DEBUG OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_definitions("-DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT")
else(NOT DEBUG OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_definitions("-DDEBUG")
endif(NOT DEBUG OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")

set(compiler_common_flags "-Wall -Wsign-compare -Wformat-security -Wpointer-arith")
set(compiler_common_flags "${compiler_common_flags} -fPIC -fdiagnostics-show-option")
set(compiler_common_flags "${compiler_common_flags} -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_REENTRANT")

set(CMAKE_C_FLAGS "${compiler_common_flags} -Wmissing-prototypes ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${compiler_common_flags} -Wnon-virtual-dtor -Woverloaded-virtual ${CMAKE_CXX_FLAGS}")

if(DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif(DEBUG)

add_executable(BeurerScaleManager ${BSM_SRCS})
target_link_libraries(BeurerScaleManager ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTSQL_LIBRARY} ${LIBUSB_LIBRARIES})
install(TARGETS BeurerScaleManager RUNTIME DESTINATION bin)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/BeurerScaleManager.doxy ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)
