# tier0.cmake

add_library( tier0 IMPORTED SHARED )

if ( UNIX )
	set( TIER0_NAME "libtier0.so" )
	configure_file( "${LIBPUBLIC}/libtier0.so" "${GAMEDIR}/bin/libtier0.so" COPYONLY )
else ()
	set( TIER0_NAME "tier0.lib" )
endif ()

set_target_properties( tier0
	PROPERTIES
		IMPORTED_IMPLIB "${LIBPUBLIC}/${TIER0_NAME}"
		IMPORTED_NO_SONAME true
)
