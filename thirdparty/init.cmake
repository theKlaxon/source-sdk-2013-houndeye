# init.cmake

# Function to add a git module as project
function( submodule name folder )
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
set( SDL_AUDIO  OFF )
set( SDL_RENDER OFF )
# `SDL_HIDAPI` is needed for bug on `src/joystick/linux/SDL_sysjoystick.c:323`,
# where an usage of `IsVirtualJoystick` (hidden behind SDL_HIDAPI) isn't disabled when the flag is not given.
set( SDL_HIDAPI ON )
set( SDL_POWER  OFF )
set( SDL_FILE   OFF )
if (WIN32)
	set( SDL_LOADSO ON )
else ()
	set( SDL_LOADSO OFF )
endif ()
set( SDL_FILESYSTEM OFF )
set( SDL_SENSOR OFF )
set( SDL_LOCALE OFF )
set( SDL_MISC   OFF )
submodule( SDL "SDL" )
# ----- VPKEdit -----
set( VPKEDIT_BUILD_CLI OFF )
set( VPKEDIT_BUILD_GUI OFF )
set( VPKEDIT_BUILD_INSTALLER OFF )
submodule( VPKEdit "VPKEdit" )
