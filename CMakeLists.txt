cmake_minimum_required( VERSION 3.0 )

project(logx VERSION 0.1)
set(Project_SHORTCUT "logx")
set(Project_PREFIX "logx")

include(GenerateExportHeader)

add_subdirectory("extras/buildx")
buildx_enable_debug()
buildx_version_dir(LOGX_VERSION_DIR)
buildx_set_default_prefixes()
buildx_set_default_output_dirs()
buildx_activate_cpp11()


set(LOGX_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
buildx_scan(LOGX_PUBLIC_INCLUDES ${LOGX_PUBLIC_INCLUDE_DIR} "hpp")
buildx_debug("public includes: ${LOGX_PUBLIC_INCLUDES}" proj)

add_subdirectory("src")
add_subdirectory("samples")
