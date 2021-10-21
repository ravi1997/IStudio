file(GLOB FILES {FILES} "*.hpp")
set(SUBDIRECTORIES "")
set(RELATIVE_PATH "CompilerError")

#Add all subdirectories 
foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	#Add source files to target
	target_sources("${PROJECT_NAME}" PUBLIC ${FILES})
	
	#Install our source files
	install(FILES ${FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${RELATIVE_PATH})
endif()