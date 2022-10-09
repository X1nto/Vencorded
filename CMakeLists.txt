cmake_minimum_required (VERSION 3.8)

project ("Vencorded")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable (Vencorded "vencorded.cpp"  "ui/options.h" "ui/options.cpp" "ui/optionsmenu.h" "ui/optionsmenu.cpp" "installer/directories.h" "installer/directories.cpp"  "installer/installer.h" "installer/installer.cpp")

target_link_libraries(Vencorded ${CONAN_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vencorded PROPERTY CXX_STANDARD 20)
endif()
