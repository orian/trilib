SET(HDRS_DIR ${PROJECT_BINARY_DIR}/include)

configure_file (
  "version.h.in"
  "${HDRS_DIR}/version.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories("${HDRS_DIR}")

SET(HDRS_CPY rbtree.h)

#file(COPY ${HDRS_CPY} DESTINATION ${HDRS_DIR})

SET(HDRS_ALL ${HDRS_CPY} "${HDRS_DIR}/version.h")

install (FILES ${HDRS_ALL} DESTINATION include/trilib/)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_testing()

  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})
  
  add_executable(rbtree_test rbtree_test.cc)
  target_link_libraries(rbtree_test ${GTEST_BOTH_LIBRARIES} glog gmock pthread)
ENDIF()