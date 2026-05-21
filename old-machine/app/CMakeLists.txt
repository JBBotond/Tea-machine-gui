cmake_minimum_required(VERSION 3.14)
project(FSM_APP C)

add_library(console_functions console_functions/devConsole.c
                              console_functions/display.c
                              console_functions/keyboard.c
                              console_functions/systemErrors.c
                              )

target_include_directories(console_functions PUBLIC console_functions ${CMAKE_CURRENT_LIST_DIR})

add_library(fsm_functions fsm_functions/fsm.c)
target_include_directories(fsm_functions PUBLIC fsm_functions ${CMAKE_CURRENT_LIST_DIR})


add_executable(app main.c
                   events.c
                   states.c)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(app PRIVATE console_functions fsm_functions)