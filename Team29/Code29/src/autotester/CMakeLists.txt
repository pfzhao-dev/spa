file(GLOB srcs 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TestWrapper.cpp")

file(GLOB headers 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AbstractWrapper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TestWrapper.h")

add_executable(autotester ${srcs})
target_link_libraries(autotester spa ${AUTOTESTER_LIBRARIES})

if (NOT WIN32)
    target_link_libraries(autotester pthread)
endif()
