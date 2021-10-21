set(FILES "")
set(SUBDIRECTORIES "IStudio")

#Add all subdirectories 
foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	#Add source files to target
	target_sources(${PROJECT_NAME} PUBLIC_HEADER ${FILES})
	
	#Install our source files
	install(FILES ${FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()