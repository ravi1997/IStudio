file(GLOB FILES {FILES} "*.hpp")
set(SUBDIRECTORIES "CompilerError" "Graph" "Manifest")

#Add all subdirectories 
foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	#Add source files to target
	target_sources("${PROJECT_NAME}" PUBLIC ${FILES})
	
	#Install our source files
	install(FILES ${FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()