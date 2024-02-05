# tier3.cmake

add_library( tier3 IMPORTED STATIC )
set_target_properties( tier3
	PROPERTIES
		IMPORTED_LOCATION "${LIBPUBLIC}/tier3${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
