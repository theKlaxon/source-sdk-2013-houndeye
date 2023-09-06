# init.cmake

function( included name path )
	# check for existance
	if ( NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${path}" )
		message( FATAL_ERROR "Thirdparty library ${name} was not found! Did you clone with `--recursive`?" )
		return()
	endif()

	add_subdirectory( "${CMAKE_CURRENT_LIST_DIR}/${path}" )
endfunction()

if ( NOT DEFINED CMAKE_TOOLCHAIN_FILE )
	message( FATAL_ERROR "vcpkg was not configured on this system, please configure it and come back" )
endif()
set( VCPKG_TARGET_ARCHITECTURE x86 )

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
message( NOTICE "vcpkg dynamic library output path is: `${VCPKG_OUTPUT_DIR}`" )

# Function used to depend on a vcpkg library
function ( target_vcpkg_dependency TARGET target VISIBILITY visibility DEPENDENCY dependency )
	find_package( ${dependency} CONFIG REQUIRED )
	add_custom_command(
		TARGET
			${target}
		COMMAND
			${CMAKE_COMMAND} -E copy_if_different "${VCPKG_OUTPUT_DIR}/${dependency}${CMAKE_SHARED_LIBRARY_SUFFIX}" "${GAMEDIR}/bin/"
	)
	target_link_libraries( ${target} ${visibility} "${dependency}::${dependency}" )
endfunction()
