# vstdlib.cmake

set( VSTDLIB_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( VSTDLIB_SOURCE_FILES
	"${VSTDLIB_DIR}/cvarsystem.cpp"

	# Header files

	# Private
	"${VSTDLIB_DIR}/cvarsystem.hpp"

	# Public
	"${SRCDIR}/public/vstdlib/vstdlib.h"
	"${SRCDIR}/public/vstdlib/vcover.h"
	"${SRCDIR}/public/vstdlib/random.h"
	"${SRCDIR}/public/vstdlib/osversion.h"
	"${SRCDIR}/public/vstdlib/jobthread.h"
	"${SRCDIR}/public/vstdlib/iprocessutils.h"
	"${SRCDIR}/public/vstdlib/IKeyValuesSystem.h"
	"${SRCDIR}/public/vstdlib/cvar.h"
	"${SRCDIR}/public/vstdlib/coroutine.h"
)

add_library( vstdlib2 SHARED ${VSTDLIB_SOURCE_FILES} )
target_compile_definitions( vstdlib2
	PRIVATE VSTDLIB_DLL_EXPORT
)


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
