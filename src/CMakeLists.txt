# grab source
buildx_scan_here(LOGX_SOURCE "cpp;hpp")
buildx_debug("source: ${LOGX_SOURCE}" proj)
buildx_auto_group("${LOGX_SOURCE}" PREFIX source)
buildx_auto_group("${LOGX_PUBLIC_INCLUDES}")

# add library
include_directories(.)
if(WIN32)
	SET(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})



add_library(logx SHARED ${LOGX_SOURCE} ${LOGX_PUBLIC_INCLUDES})
target_link_libraries(logx ${Boost_LIBRARIES})
buildx_export_header(logx
					DESTINATION_FILE "LOGX_PUBLIC_INCLUDES"
					DESTINATION_DIR "LOGX_EXPORT_API_DIR"
					SUB_DIR logx)

install(TARGETS logx EXPORT logxTargets
  LIBRARY DESTINATION lib/${LOGX_VERSION_DIR}
  ARCHIVE DESTINATION lib/${LOGX_VERSION_DIR}
  RUNTIME DESTINATION bin/${LOGX_VERSION_DIR}
)

buildx_target_includes(	TARGETS logx
						FILES ${LOGX_PUBLIC_INCLUDES}
						BASE_DIRECTORIES
							${LOGX_PUBLIC_INCLUDE_DIR}
							${LOGX_EXPORT_API_DIR}
						DESTINATION include/${LOGX_VERSION_DIR})

buildx_add_external_test(external-test ${PROJECT_SOURCE_DIR}/tests/interface TEST_TARGETS logx)
buildx_add_internal_test(internal-test ${PROJECT_SOURCE_DIR}/tests/internal TEST_TARGET logx)