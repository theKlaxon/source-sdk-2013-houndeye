# vvis_launcher.cmake

set( VVIS_LAUNCHER_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( VVIS_LAUNCHER_SOURCE_FILES
	"${VVIS_LAUNCHER_DIR}/vvis_launcher.cpp"

	# Header Files
	"${SRCDIR}/public/tier1/interface.h"
)

add_executable( vvis_launcher ${VVIS_LAUNCHER_SOURCE_FILES} )

set_target_properties( vvis_launcher
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${GAMEDIR}/bin"
)

target_include_directories( vvis_launcher
	PRIVATE
		"${SRCDIR}/utils/common"
)

if (WIN32)
	target_link_options( vvis_launcher
		PRIVATE
			/LARGEADDRESSAWARE
	)
endif ()

target_link_libraries( vvis_launcher
	PRIVATE
		${ASRC_tier02}
		tier1
		${ASRC_vstdlib2}
		${CMAKE_DL_LIBS}
)
add_dependencies( vvis_launcher vvis_dll )
