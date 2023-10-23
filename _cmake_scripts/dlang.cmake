# dlang.cmake

function( add_dlang_project NAMED Name IN ProjectDir )
	add_custom_target( ${Name}
		COMMAND
			dub build --quiet --arch=x86
		DEPENDS
			"${GAMEDIR}/bin/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}"
			"${GAMEDIR}/bin/druntime-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}"
		WORKING_DIRECTORY
			${ProjectDir}
	)
endfunction()

set( LDC2_ROOT "$ENV{LDC2_ROOT}" CACHE PATH "The path to the ldc2 root" )
if ( LDC2_ROOT STREQUAL "" )
	if ( NOT WIN32 )
		message( WARNING "Using D on linux is not tested! Here be dragons." )
	else()
		message( FATAL_ERROR "`LDC2_ROOT` was not provided, aborting." )
	endif()
endif()

configure_file( "${LDC2_ROOT}/lib32/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" "${GAMEDIR}/bin/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" COPYONLY )
configure_file( "${LDC2_ROOT}/lib32/druntime-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" "${GAMEDIR}/bin/druntime-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" COPYONLY )