add_executable(server_test)

target_sources(server_test
	PRIVATE
		server_test.c
)

target_link_libraries(server_test
	PRIVATE
		server	
)

add_executable(linkedlist_test)

target_sources(linkedlist_test
	PRIVATE
		linkedlist_test.c
)

target_link_libraries(linkedlist_test
	PRIVATE
		linkedlist
)