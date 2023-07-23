# init.cmake

function( thirdparty name path )
	# check for existance
	if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${path}" )
		message( FATAL_ERROR "Thirdparty library ${name} was not found! Did you clone with `--recursive`?" )
		return()
	endif()

	add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${path}" )
endfunction()

