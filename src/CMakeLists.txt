add_library(TicTac INTERFACE)
target_sources(TicTac INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tictac.h)
target_compile_features(TicTac INTERFACE cxx_std_17)

add_executable(tictactoe main.cpp)
target_link_libraries(tictactoe PUBLIC TicTac)
target_link_libraries(tictactoe PUBLIC ${READLINE_LIBRARIES})
target_include_directories(tictactoe PUBLIC ${READLINE_INCLUDE_DIR})

add_executable(unittest test_tictac.cpp)
target_link_libraries(unittest PUBLIC Catch2 TicTac)
