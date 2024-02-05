# tier2.cmake

add_library( tier2 IMPORTED STATIC )
set_target_properties( tier2
	PROPERTIES
		IMPORTED_LOCATION "${LIBPUBLIC}/"
)
