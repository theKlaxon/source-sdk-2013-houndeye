# bootstrap.cmake

set( BOOTSTRAP_DIR ${CMAKE_CURRENT_LIST_DIR} )

add_custom_target( bootstrap
	COMMAND
		"dub build --arch=x86"
	DEPENDS
		"${GAMEDIR}/bin/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}"
	WORKING_DIRECTORY
		${BOOTSTRAP_DIR}
)

set( PHOBOS2_PATH "" )

# FIXME: Finding the phobos-shared-ting ain't working: please fix
find_path( PHOBOS2_PATH "dub${CMAKE_EXECUTABLE_SUFFIX}" PATHS "$ENV{Path}" NO_CACHE REQUIRED )
message( FATAL_ERROR "`${PHOBOS2_PATH}`" )
add_custom_command(
	OUTPUT
		"${GAMEDIR}/bin/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}"
	COMMAND
		"${CMAKE_EXECUTABLE} -E copy_if_different ${PHOBOS2_PATH} ${GAMEDIR}/bin/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}"
)
