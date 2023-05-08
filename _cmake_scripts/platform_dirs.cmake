# platform_dirs.cmake

if (UNIX)
	set(PLATSUBDIR "/linux32")
elseif (WIN32)
	set(PLATSUBDIR "/.")
endif()