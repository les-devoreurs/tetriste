cmake_minimum_required(VERSION 3.7)

project(Arkanoid LANGUAGES C)

IF (MSVC)
	set (CMAKE_PREFIX_PATH "c:/sdl2")
ENDIF()

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} "")
target_sources(${PROJECT_NAME} PUBLIC arkanoid.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sprites.bmp
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)	

IF (MSVC)
	file(COPY ${SDL2_LIBDIR}/SDL2.dll
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/)
	file(COPY ${SDL2_LIBDIR}/SDL2.dll
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
ENDIF()
