add_library("${PROJECT_NAME}" STATIC "")
add_library(IStudio::IStudio ALIAS "${PROJECT_NAME}")

target_include_directories("${PROJECT_NAME}" PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>" #SymbolTable_export.h
	"$<INSTALL_INTERFACE:include>" #${CMAKE_INSTALL_PREFIX}/include
)

if(NOT BUILD_SHARED_LIBS)
	set(STATIC_POSTFIX "-s")
	set_target_properties("${PROJECT_NAME}" PROPERTIES 
	COMPILE_FLAGS -DSYMBOLTABLE_STATIC_DEFINE)
endif()

#Rename target SYMBOLTABLE
set_target_properties("${PROJECT_NAME}" PROPERTIES
	DEBUG_POSTFIX "${STATIC_POSTFIX}-d" 
	RELEASE_POSTFIX "${STATIC_POSTFIX}"
	MINSIZEREL_POSTFIX "${STATIC_POSTFIX}-mr"
	RELWITHDEBINFO_POSTFIX "${STATIC_POSTFIX}-rd"
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
set ( PROJECT_LINK_LIBS libSymbolTable.a libLocation.a libLogger.a libTypes.a libProperty.a libCLI.a)
link_directories(${CMAKE_INSTALL_LIBDIR})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${PROJECT_LINK_LIBS})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        PUBLIC_HEADER
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
add_subdirectory("include")
add_subdirectory("source")