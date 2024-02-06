# launcher.cmake

set( LAUNCHER_DIR ${CMAKE_CURRENT_LIST_DIR} )
set(
	LAUNCHER_SOURCE_FILES
		# Project files
		"${LAUNCHER_DIR}/launcher.cpp"
		"${LAUNCHER_DIR}/launcher.hpp"
		"${LAUNCHER_DIR}/sourceinit.cpp"
		"${LAUNCHER_DIR}/sourceinit.hpp"

		# Common files
		"${SRCDIR}/public/filesystem_init.cpp"
)

add_library( launcher MODULE ${LAUNCHER_SOURCE_FILES} )

set_target_properties(
	launcher PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY "${GAMEDIR}/bin"
		PREFIX ""
)

target_include_directories(
	launcher PRIVATE
		"${SRCDIR}/common"
		"${SRCDIR}/public"
)
target_link_libraries(
	launcher PRIVATE
#		appframework
		tier0
		tier1
		tier2
		tier3
		vstdlib
)
