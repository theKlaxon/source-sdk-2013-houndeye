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
if ( WIN32 )
	if ( LDC2_ROOT STREQUAL "" )
		message( FATAL_ERROR "`LDC2_ROOT` was not provided, aborting." )
	endif ()
	set( DLIB_DIR "${LDC2_ROOT}/lib32" )
else()
	message( WARNING "Using D on ${CMAKE_SYSTEM_NAME} is not tested! Here be dragons." )
	set( DLIB_DIR "/lib" )
endif ()

configure_file( "${DLIB_DIR}/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" "${GAMEDIR}/bin/phobos2-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" COPYONLY )
configure_file( "${DLIB_DIR}/druntime-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" "${GAMEDIR}/bin/druntime-ldc-shared${CMAKE_SHARED_LIBRARY_SUFFIX}" COPYONLY )
