# launcher.cmake

set( LAUNCHER_DIR ${CMAKE_CURRENT_LIST_DIR} )
set(
	LAUNCHER_SOURCE_FILES
		"${LAUNCHER_DIR}/launcher_ds.cpp"
		"${LAUNCHER_DIR}/launcher.cpp"
		"${LAUNCHER_DIR}/launcher.hpp"
)

add_library( launcher MODULE ${LAUNCHER_SOURCE_FILES} )

set_target_properties(
	launcher PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY "${GAMEDIR}/bin"
)

target_include_directories(
	launcher PRIVATE
		"${SRCDIR}/common"
		"${SRCDIR}/public"
)
target_link_libraries(
	launcher PRIVATE
		appframework
		tier0
		tier1
		tier2
		vstdlib
)
