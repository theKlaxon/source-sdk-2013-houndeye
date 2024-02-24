# raytrace.cmake

#enable_language(CUDA)
#
#set( RAYTRACE_CUDA_DIR ${CMAKE_CURRENT_LIST_DIR} )
#set( RAYTRACE_CUDA_SOURCE_FILES
#	"${RAYTRACE_CUDA_DIR}/raytrace.cpp"
#	"${RAYTRACE_CUDA_DIR}/trace2.cpp"
#	"${RAYTRACE_CUDA_DIR}/trace3.cpp"
#)
#
#cuda_add_library( raytrace_cuda STATIC ${RAYTRACE_CUDA_SOURCE_FILES} )
#target_include_directories( raytrace_cuda
#	PRIVATE "${SRCDIR}/utils/common"
#)