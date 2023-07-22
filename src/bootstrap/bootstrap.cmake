# bootstrap.cmake

set( BOOTSTRAP_DIR ${CMAKE_CURRENT_LIST_DIR} )
set(
	BOOTSTRAP_SOURCE_FILES
		"${BOOTSTRAP_DIR}/main.cpp"

		# Header Files

)

add_executable( bootstrap ${BOOTSTRAP_SOURCE_FILES} )

set_target_properties(
	bootstrap PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${GAMEDIR}/"
		RUNTIME_OUTPUT_NAME "aurosrc"
)
