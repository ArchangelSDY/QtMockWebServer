# set all include directories for in and out of source builds
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(CMAKE_AUTOMOC ON)

file(GLOB SRCS "*.cpp")
file(GLOB PUBLIC_HEADERS "*.h")

add_library(qtmockwebserver ${SRCS})
set_target_properties(qtmockwebserver PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_link_libraries(qtmockwebserver Qt5::Core Qt5::Network)

if(BUILD_SHARED_LIBS)
    install(FILES ${PUBLIC_HEADERS} DESTINATION include/qtmockwebserver)
    install(TARGETS qtmockwebserver LIBRARY DESTINATION ${LIB_DESTINATION} ARCHIVE DESTINATION ${LIB_DESTINATION} RUNTIME DESTINATION ${LIB_DESTINATION})
endif(BUILD_SHARED_LIBS)