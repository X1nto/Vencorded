cmake_minimum_required (VERSION 3.8)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")

project ("Vencorded")

add_executable (Vencorded "vencorded.cpp"  "ui/options.h" "ui/options.cpp" "ui/optionsmenu.h" "ui/optionsmenu.cpp" "installer/directories.h" "installer/directories.cpp"  "installer/installer.h" "installer/installer.cpp")

if (WIN32)
	find_path(PDC_INCLUDES curses.h REQUIRED)
	find_library(PDC_LIBS pdcurses REQUIRED)
	include_directories(${PDC_INCLUDES})

	target_include_directories(Vencorded PRIVATE ${PDC_INCLUDES})
	target_link_libraries(Vencorded ${PDC_LIBS})
else()
	find_package(Curses REQUIRED)
	include_directories(${CURSES_INCLUDE_DIR})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vencorded PROPERTY CXX_STANDARD 20)
endif()
