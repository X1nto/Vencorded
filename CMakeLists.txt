cmake_minimum_required (VERSION 3.8)

set(VCPKG_TARGET_TRIPLET "x64-windows-static")

project ("Vencorded")

add_executable (Vencorded 
	"vencorded.cpp" 

	"ui/options.h"
	"ui/options.cpp" 
	"ui/optionsmenu.h"
	"ui/optionsmenu.cpp"

	"installer/installer.h" 
	"installer/installer.cpp"
	"downloader/downloader.cpp"
	"directories/discord.h"
	"directories/discord.cpp"
 "downloader/downloader.h" "directories/vencorded.h" "directories/vencorded.cpp")

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

find_package(unofficial-git2 CONFIG REQUIRED)
target_link_libraries(Vencorded unofficial::git2::git2)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vencorded PROPERTY CXX_STANDARD 20)
endif()
