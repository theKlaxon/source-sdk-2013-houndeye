# bitmap.cmake

set( BITMAP_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( BITMAP_SOURCE_FILES
	# Sources
	"${BITMAP_DIR}/bitmap.cpp"
	"${BITMAP_DIR}/tgaloader.cpp"
	"${BITMAP_DIR}/tgawriter.cpp"

	# Public units

	# Public headers
	"${SRCDIR}/public/bitmap/bitmap.h"
	"${SRCDIR}/public/bitmap/cubemap.h"
	"${SRCDIR}/public/bitmap/float_bm.h"
	"${SRCDIR}/public/bitmap/imageformat.h"
	"${SRCDIR}/public/bitmap/psd.h"
	"${SRCDIR}/public/bitmap/tgaloader.h"
	"${SRCDIR}/public/bitmap/tgawriter.h"
	"${SRCDIR}/public/filesystem.h"
)

add_library( bitmap STATIC ${BITMAP_SOURCE_FILES} )

target_include_directories( bitmap
	PRIVATE
		"${SRCDIR}/common"
		"${SRCDIR}/public"
)
target_link_libraries( bitmap
	PRIVATE
		tier0
		vstdlib
)
