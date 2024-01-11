# FIXME: Apparently declaring this makes `{vvis,vrad}_launcher` depend on the explicit path of the `libtier0.so` library, making it fail to load `{vrad,vvis}_dll.so`...??
#add_library( tier0 IMPORTED SHARED )
#set( TIER0_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}tier0${CMAKE_SHARED_LIBRARY_SUFFIX} )
#set_property( TARGET tier0 PROPERTY IMPORTED_IMPLIB "${LIBPUBLIC}/${TIER0_NAME}" )
#configure_file( "${LIBPUBLIC}/${TIER0_NAME}" "${GAMEDIR}/bin/${TIER0_NAME}" COPYONLY )
