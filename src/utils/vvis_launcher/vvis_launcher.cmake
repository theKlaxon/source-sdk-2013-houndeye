# vvis_launcher.cmake

set(VVIS_LAUNCHER_DIR ${CMAKE_CURRENT_LIST_DIR})
set(
	VVIS_LAUNCHER_SOURCE_FILES

	"${VVIS_LAUNCHER_DIR}/vvis_launcher.cpp"
	"${VVIS_LAUNCHER_DIR}/stdafx.cpp"

	# Header Files
	"${SRCDIR}/public/tier1/interface.h"
	"${VVIS_LAUNCHER_DIR}/stdafx.h"
)

add_executable(vvis_launcher ${VVIS_LAUNCHER_SOURCE_FILES})

set_target_properties( vvis_launcher
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${GAMEDIR}/bin"
)

target_precompile_headers( vvis_launcher
	PRIVATE
		"${VVIS_LAUNCHER_DIR}/stdafx.h"
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

add_library( tier0 IMPORTED SHARED )
set_property( TARGET tier0 PROPERTY IMPORTED_IMPLIB "${LIBPUBLIC}/${CMAKE_SHARED_LIBRARY_PREFIX}tier0${CMAKE_SHARED_LIBRARY_SUFFIX}" )
add_library( vstdlib IMPORTED SHARED )
set_property( TARGET vstdlib PROPERTY IMPORTED_IMPLIB "${LIBPUBLIC}/${CMAKE_SHARED_LIBRARY_PREFIX}vstdlib${CMAKE_SHARED_LIBRARY_SUFFIX}" )

#target_link_libraries( vvis_launcher
#	PRIVATE
#		tier0
#		tier1
#		vstdlib
#)
add_dependencies( vvis_launcher vvis_dll )
