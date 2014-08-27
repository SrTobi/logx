
################### scan for source ##################

SCAN_SOURCE_HERE(SOURCE "cpp;hpp")
AUTO_SOURCE_GROUP("${SOURCE}")

AUTO_SOURCE_GROUP("${LOGX_INCLUDE_FILES}")

add_executable(logx-simple ${SOURCE} ${LOGX_INCLUDE_FILES})
target_link_libraries(logx-simple ${Boost_LIBRARIES})