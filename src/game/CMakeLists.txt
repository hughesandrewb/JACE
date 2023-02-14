file(GLOB src_files *.cc)
set(game_source_files ${src_files})
add_library(game ${game_source_files})
target_link_libraries(game arch)
