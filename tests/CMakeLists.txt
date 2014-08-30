
include_directories(.)
SCAN_SOURCE_HERE(TEST_SOURCE "cpp;hpp")

AUTO_SOURCE_GROUP("${TEST_SOURCE}")
AUTO_SOURCE_GROUP("${LOGX_INCLUDE_FILES}")

add_executable(logx-tests ${TEST_SOURCE} ${LOGX_INCLUDE_FILES})
set_target_properties(logx-tests PROPERTIES COMPILE_DEFINITIONS "LOGX_TEST")
target_link_libraries(logx-tests ${Boost_LIBRARIES})


add_executable(wlogx-tests ${TEST_SOURCE} ${LOGX_INCLUDE_FILES})
set_target_properties(wlogx-tests PROPERTIES COMPILE_DEFINITIONS "LOGX_TEST;LOGXCFG_USE_WCHAR")
target_link_libraries(wlogx-tests ${Boost_LIBRARIES})