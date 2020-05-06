include(CMakeFindDependencyMacro)
find_dependency(JPEG)
find_dependency(PNG)
include("${CMAKE_CURRENT_LIST_DIR}/imgppTargets.cmake")