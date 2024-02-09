# init.cmake

# Function to add a git module as project
function( included name folder )
	# check for existence
	if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${folder}/CMakeLists.txt" )
		message( FATAL_ERROR "Third-party submodule library `${name}` was not found! Did you clone with `--recursive`?\nNote: you may run `git submodule init && git submodule update` to fix" )
	endif()

	add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${folder}" EXCLUDE_FROM_ALL SYSTEM )
endfunction()


# For gmodules, declare them here, for vcpkg deps, declare them directly in the dependant project
included( SDL "SDL" )
