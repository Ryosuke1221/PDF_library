add_executable(test_libharu test_libharu.cpp )

include_directories("${CMAKE_SOURCE_DIR}/libharu_include" )

set(LIBHARU_LIB_FILES 
	${CMAKE_SOURCE_DIR}/libharu_lib_dll/libhpdfd.lib
	${CMAKE_SOURCE_DIR}/libharu_lib_dll/libhpdfsd.lib
	${CMAKE_SOURCE_DIR}/libharu_lib_dll/libhpdf.lib
	${CMAKE_SOURCE_DIR}/libharu_lib_dll/libhpdfs.lib
)

target_link_libraries(test_libharu ${LIBHARU_LIB_FILES} )

set(RESOURCE_FILES ${CMAKE_SOURCE_DIR}/libharu_lib_dll/libhpdfd.dll)

file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
