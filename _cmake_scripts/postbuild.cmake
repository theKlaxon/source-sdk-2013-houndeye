# postbuild.cmake

include_guard(GLOBAL)

function(target_strip_symbols target)
	if ( ${IS_LINUX} )
		add_custom_command(
			TARGET ${target}
			POST_BUILD
				COMMAND ${CMAKE_OBJCOPY} "$<TARGET_FILE:${target}>" "$<TARGET_FILE:${target}>.dbg"
				COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink="$<TARGET_FILE:${target}>.dbg" "$<TARGET_FILE:${target}>"
				COMMAND ${CMAKE_STRIP} -x "$<TARGET_FILE:${target}>" $<$<CONFIG:Release>:-S>
		)
	endif()
endfunction()

# Creates a symlink from the target's output binary to the game/bin directory
function(link_to_bin)
	cmake_parse_arguments( CTB "" "TARGET" "" ${ARGN} )
	if ( NOT DEFINED "CTB_TARGET" )
		message( SEND_ERROR "Missing `TARGET` parameter!" )
		return()
	endif ()

	add_custom_command(
		TARGET ${CTB_TARGET}
		POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E $<IF:$<BOOL:WIN32>,copy,create_symlink> $<TARGET_FILE:${CTB_TARGET}> ${GAMEDIR}/bin/$<TARGET_FILE_NAME:${CTB_TARGET}>
	)
endfunction()

# Declares a shared library other can link to.
# As example, to link to `tier0` you can do target_link_libraries( ${target} ${vis} ${ASRC_tier0} )`
function(declare_library)
	cmake_parse_arguments( DL "" "TARGET" "" ${ARGN} )
	if ( NOT DEFINED "DL_TARGET" )
		message( SEND_ERROR "Missing `TARGET` parameter!" )
		return()
	endif ()

	set( "ASRC_${DL_TARGET}" $<IF:$<BOOL:WIN32>,$<TARGET_NAME:${DL_TARGET}>,$<TARGET_FILE:${DL_TARGET}>> PARENT_SCOPE )
endfunction()