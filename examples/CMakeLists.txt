include_directories(${CMAKE_SOURCE_DIR}/include)
set ( PROJECT_LINK_LIBS libIStudio.a )
link_directories(${CMAKE_SOURCE_DIR}/lib)


add_executable("Example" "Main.cpp")
#add_library(SymbolTable::SymbolTable STATIC IMPORTED)
target_link_libraries("Example" LINK_PUBLIC  ${PROJECT_NAME} ${PROJECT_LINK_LIBS})
