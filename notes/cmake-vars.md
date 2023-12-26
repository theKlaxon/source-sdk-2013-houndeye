options:
  - RETAIL: retail build mode
  - STAGING_ONLY: staging only
_cmake_scripts globals:
  - IS_SOURCESDK = 1
  - RAD_TELEMETRY_DISABLED = ${IS_SOURCESDK}
  - TF_BETA = 0
  - BUILD_REPLAY = 0
  - DEDICATED = 0
compile definitions:
  - _RETAIL? ${RETAIL}
  - STAGING_ONLY?
  - TF_BETA?
  - RAD_TELEMETRY_DISABLED?
  - _DLLEXT = ".dll" | ".dylib" | ".so"
  - FRAME_POINTER_OMISSION_DISABLED