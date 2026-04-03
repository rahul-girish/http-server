add_executable(server_test)

target_sources(server_test
	PRIVATE
		server_test.c
)

target_link_libraries(server_test
	PRIVATE
		server	
)