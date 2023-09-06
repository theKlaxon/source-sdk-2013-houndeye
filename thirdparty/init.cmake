# init.cmake

function( included name path )
	# check for existance
	if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${path}" )
		message( FATAL_ERROR "Thirdparty library ${name} was not found! Did you clone with `--recursive`?" )
		return()
	endif()

	add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${path}" )
endfunction()

if( NOT DEFINED CMAKE_TOOLCHAIN_FILE )
	message( FATAL_ERROR "vcpkg was not configured on this system, please configure it and come back" )
endif()
set( VCPKG_TARGET_ARCHITECTURE x86 )


find_package( SDL2 CONFIG REQUIRED ) # target_link_libraries( $modulename PRIVATE SDL2::SDL2 )
#add_custom_command(
#	TARGET
#		SDL2::SDL2
#	COMMAND
#		${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2> game/bin
#)


