# tier2.cmake

add_library( tier2 IMPORTED SHARED )
set_target_properties( tier2
	PROPERTIES
		IMPORTED_IMPLIB "${LIBPUBLIC}/tier2${CMAKE_STATIC_LIBRARY_SUFFIX}"
)
