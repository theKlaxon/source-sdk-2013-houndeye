# vstdlib.cmake

add_library( vstdlib IMPORTED SHARED )

if ( UNIX )
	set( VSTDLIB_NAME "libvstdlib.so" )
	configure_file( "${LIBPUBLIC}/libvstdlib.so" "${GAMEDIR}/bin/libvstdlib.so" COPYONLY )
else ()
	set( VSTDLIB_NAME "vstdlib.lib" )
endif ()

set_target_properties( vstdlib
	PROPERTIES
		IMPORTED_IMPLIB "${LIBPUBLIC}/${VSTDLIB_NAME}"
		IMPORTED_NO_SONAME true
)
