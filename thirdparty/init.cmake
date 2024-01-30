# init.cmake

# Function to add a git module as project
function( included name folder )
	# check for existance
	if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${folder}" )
		message( FATAL_ERROR "Thirdparty library ${name} was not found! Did you clone with `--recursive`?" )
		return()
	endif()

	add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${folder}" )
endfunction()

# Find vcpkg dynlib output dir
foreach ( path ${CMAKE_PREFIX_PATH} )
	STRING( FIND ${path} "/debug" isDebugFolder )

	if ( ${isDebugFolder} EQUAL -1 )
		set( VCPKG_OUTPUT_DIR "${path}/bin" )
	endif()
endforeach()

if ( NOT DEFINED VCPKG_OUTPUT_DIR )
	message( FATAL_ERROR "No vcpkg dynamic library output path was found" )
endif()
message( STATUS "vcpkg dynamic library output path is: `${VCPKG_OUTPUT_DIR}`" )


# For gmodules, declare them here, for vcpkg deps, declare them directly in the dependant project
included( SDL "SDL" )