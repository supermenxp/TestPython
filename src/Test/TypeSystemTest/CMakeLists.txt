project(TypeSystemTest)

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.h *.ui)
source_group_by_dir(SRC)

# message(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SRC})