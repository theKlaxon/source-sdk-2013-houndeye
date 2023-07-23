# bootstrap.cmake

set( BOOTSTRAP_DIR ${CMAKE_CURRENT_LIST_DIR} )

add_custom_target( bootstrap
	COMMAND dub build --arch=x86
	WORKING_DIRECTORY ${BOOTSTRAP_DIR}
)
