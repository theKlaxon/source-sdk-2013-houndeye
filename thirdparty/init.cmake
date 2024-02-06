# init.cmake

# Function to add a git module as project
function( included name folder )
	# check for existance
	if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${folder}" )
		message( FATAL_ERROR "Thirdparty library ${name} was not found! Did you clone with `--recursive`?" )
		return()
	endif()

	add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${folder}" EXCLUDE_FROM_ALL SYSTEM )
endfunction()


# For gmodules, declare them here, for vcpkg deps, declare them directly in the dependant project
included( SDL "SDL" )
