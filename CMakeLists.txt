cmake_minimum_required(VERSION 3.2)
enable_testing()

if($ENV{TRAVIS_BUILD_NUMBER})
    project(otusip VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(otusip VERSION 0.0.1001)
endif()

if(${CMAKE_MINOR_VERSION} GREATER 10)
        cmake_policy(SET CMP0074 NEW)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
link_directories(${Boost_LIBRARY_DIR})

configure_file(tmpl/version.h.in ${PROJECT_SOURCE_DIR}/src/version.h)

add_library(ip_utils src/ip_utils.cpp)
add_executable(ip_filter       src/main.cpp)
add_executable(otusip_tests tests/main_tests.cpp src/ip_utils.cpp)

set_target_properties(ip_filter ip_utils otusip_tests PROPERTIES
			CXX_STANDARD 14
			CXX_STANDARD_REQUIRED YES
                        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-std=c++14"
                     )
set_target_properties(  otusip_tests PROPERTIES
                                     COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
                                     INCLUDE_DIRECTORIES    ${Boost_INCLUDE_DIR}
    )
target_include_directories(otusip_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(ip_filter ip_utils)
target_link_libraries(otusip_tests ip_utils ${Boost_LIBRARIES})

install(TARGETS ip_filter RUNTIME DESTINATION bin)
set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT hard.slot@gmail.com)
include (CPack)


add_test(test_version_valid otusip_tests)
