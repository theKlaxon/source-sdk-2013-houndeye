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
# ----- SDL -----
set( SDL_STATIC OFF )
set( SDL_SHARED ON )
set( SDL_ATOMIC OFF )
set( SDL_AUDIO OFF )
set( SDL_RENDER OFF )
set( SDL_HIDAPI OFF )
set( SDL_POWER OFF )
set( SDL_FILE OFF )
set( SDL_LOADSO OFF )
set( SDL_FILESYSTEM OFF )
set( SDL_SENSOR OFF )
set( SDL_LOCALE OFF )
set( SDL_MISC OFF )
included( SDL "SDL" )
# ----- VPKEdit -----
set( VPKEDIT_BUILD_CLI OFF )
set( VPKEDIT_BUILD_GUI OFF )
set( VPKEDIT_BUILD_INSTALLER OFF )
included( VPKEdit "VPKEdit" )
