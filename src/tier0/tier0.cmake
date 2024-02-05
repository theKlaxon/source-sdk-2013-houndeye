# tier0.cmake

add_library( tier0 IMPORTED SHARED )

if ( UNIX )
	set( TIER0_NAME "libtier0.so" )
else ()
	set( TIER0_NAME "tier0.lib" )
endif ()

set_target_properties( tier0
	PROPERTIES
		IMPORTED_IMPLIB "${LIBPUBLIC}/${TIER0_NAME}"
		IMPORTED_NO_SONAME true
)
configure_file( "${LIBPUBLIC}/${TIER0_NAME}" "${GAMEDIR}/bin/${TIER0_NAME}" COPYONLY )
