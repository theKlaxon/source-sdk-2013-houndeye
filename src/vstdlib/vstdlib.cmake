# vstdlib.cmake

add_library( vstdlib IMPORTED SHARED )

if ( UNIX )
	set( VSTDLIB_NAME "libvstdlib.so" )
else ()
	set( VSTDLIB_NAME "vstdlib.lib" )
endif ()

set_target_properties( vstdlib
	PROPERTIES
		IMPORTED_IMPLIB "${LIBPUBLIC}/${VSTDLIB_NAME}"
		IMPORTED_NO_SONAME true
)
configure_file( "${LIBPUBLIC}/${VSTDLIB_NAME}" "${GAMEDIR}/bin/${VSTDLIB_NAME}" COPYONLY )
