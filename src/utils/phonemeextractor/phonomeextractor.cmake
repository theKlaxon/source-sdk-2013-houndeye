# phonomeextractor.cmake

set(PHONOMEEXTRACTOR_DIR ${CMAKE_CURRENT_LIST_DIR})
set(
	PHONOMEEXTRACTOR_SOURCE_FILES

	"${PHONOMEEXTRACTOR_DIR}/phonomeextractor.cpp"
)

add_executable(phonomeextractor ${PHONOMEEXTRACTOR_SOURCE_FILES})

set_target_properties(
	phonomeextractor PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${GAMEDIR}/bin"
)

target_link_libraries(
	phonomeextractor PRIVATE
		mathlib
)