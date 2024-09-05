# launcher.cmake

set( LAUNCHER_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( LAUNCHER_SOURCE_FILES
	# Project files
	"${LAUNCHER_DIR}/launcher.cpp"
	"${LAUNCHER_DIR}/gameapploader.cpp"
	"${LAUNCHER_DIR}/gameapploader.hpp"

	# Common files
	"${SRCDIR}/public/filesystem_init.cpp"
	"${SRCDIR}/public/idedicatedexports.h"
)

add_library( launcher MODULE ${LAUNCHER_SOURCE_FILES} )

set_target_properties( launcher
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY "${GAMEDIR}/bin"
		PREFIX ""
)

target_include_directories( launcher
	PRIVATE
		"${SRCDIR}/common"
		"${SRCDIR}/public"
)
target_link_libraries( launcher
	PRIVATE
		appframework
		tier0
		tier1
		tier2
		tier3
		mathlib
		vstdlib
)
