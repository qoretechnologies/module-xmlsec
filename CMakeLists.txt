cmake_minimum_required(VERSION 3.5...3.31)

project(qore-xmlsec-module)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (VERSION_PATCH 1)

set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Qore 1.0 REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(XMLSec REQUIRED)

option(QORE_XMLSEC_STRICT_DOCS "Fail XML Security documentation builds on warnings" OFF)

list(APPEND CMAKE_REQUIRED_LIBRARIES ${LIBXML2_LIBRARIES})

list(APPEND CMAKE_REQUIRED_LIBRARIES ${XMLSEC1_LIBRARIES})
list(APPEND CMAKE_REQUIRED_LIBRARIES ${XMLSEC1_OPENSSL_LIBRARIES})

list(APPEND CMAKE_REQUIRED_INCLUDES ${LIBXML2_INCLUDE_DIR})

list(APPEND CMAKE_REQUIRED_INCLUDES ${XMLSEC1_INCLUDE_DIR})
list(APPEND CMAKE_REQUIRED_INCLUDES ${XMLSEC1_OPENSSL_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${XMLSEC1_INCLUDE_DIR})
include_directories(${LIBXML2_INCLUDE_DIR})
include_directories(${QORE_INCLUDE_DIR})

# Check for C++11.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${XMLSEC_CFLAGS} ${XMLSEC1_OPENSSL_CFLAGS}")

if (DEFINED ENV{CXXFLAGS})
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")
endif()

if (DEFINED ENV{XMLSEC_CXXFLAGS})
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{XMLSEC_CXXFLAGS}")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include_directories(${XMLSEC_INCLUDE_DIR})

set(QPP_SRC
    src/QC_XmlSec.qpp
    src/QC_XmlSecKey.qpp
    src/QC_XmlSecKeyManager.qpp
)

set(CPP_SRC
    src/xmlsec.cpp
)

set(QMOD
)

qore_wrap_qpp_value(QPP_SOURCES METALIST QPP_META_SRC ${QPP_SRC})

SET (module_name "xmlsec")

set(QORE_DOX_TMPL_SRC
  docs/mainpage.dox.tmpl
    docs/guide-xmlsec_errors.dox.tmpl
    docs/guide-xmlsec_examples.dox.tmpl
    docs/guide-xmlsec_key_types.dox.tmpl
    docs/guide-xmlsecreleasenotes.dox.tmpl
)

add_library(${module_name} MODULE ${QPP_SOURCES} ${CPP_SRC})

if (DEFINED ENV{DOXYGEN_EXECUTABLE})
    set(DOXYGEN_EXECUTABLE $ENV{DOXYGEN_EXECUTABLE})
endif()

# Restrict the public reference to generated QPP declarations and the main page.
set(MODULE_DOX_INPUT "${CMAKE_CURRENT_BINARY_DIR}/mainpage.dox")
foreach(_xmlsec_qpp IN LISTS QPP_SRC)
    get_filename_component(_xmlsec_name "${_xmlsec_qpp}" NAME_WE)
    list(APPEND MODULE_DOX_INPUT "${CMAKE_CURRENT_BINARY_DIR}/${_xmlsec_name}.dox.h")
endforeach()
string(REPLACE ";" " " MODULE_DOX_INPUT "${MODULE_DOX_INPUT}")

list(APPEND MODULE_DOX_INPUT
    ${CMAKE_CURRENT_BINARY_DIR}/guide-xmlsec_errors.dox
    ${CMAKE_CURRENT_BINARY_DIR}/guide-xmlsec_examples.dox
    ${CMAKE_CURRENT_BINARY_DIR}/guide-xmlsec_key_types.dox
    ${CMAKE_CURRENT_BINARY_DIR}/guide-xmlsecreleasenotes.dox
)
qore_external_binary_module(${module_name} ${PROJECT_VERSION} "${XMLSEC1_LIBRARIES}" "${XMLSEC1_OPENSSL_LIBRARIES}")
#qore_user_modules("${QMOD}")
install(PROGRAMS ${SCRIPTS} DESTINATION bin)

qore_dist(${PROJECT_VERSION})

# install qpp metadata files
if (COMMAND qore_install_qpp_metadata)
    qore_install_qpp_metadata(${module_name} ${QPP_META_SRC})
else()
    install(FILES ${QPP_META_SRC}
            DESTINATION ${CMAKE_INSTALL_DATADIR}/qore/metadata)
endif()

qore_config_info()

if (DOXYGEN_FOUND)
    qore_wrap_dox(QORE_DOX_SRC ${QORE_DOX_TMPL_SRC})
    add_custom_target(QORE_MOD_DOX_FILES DEPENDS ${QORE_DOX_SRC})
    add_dependencies(docs-module QORE_MOD_DOX_FILES)
    if (QORE_XMLSEC_STRICT_DOCS)
        file(APPEND "${CMAKE_BINARY_DIR}/Doxyfile" "\nWARN_AS_ERROR = FAIL_ON_WARNINGS\n")
    endif()
endif()
