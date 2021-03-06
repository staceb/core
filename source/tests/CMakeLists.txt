#
# Setup test environment
#

# Check if tests are enabled
if(NOT OPTION_BUILD_TESTS)
	return()
endif()

# DISABLE CMP0037: Target names should not be reserved and should match a validity pattern
if("${CMAKE_VERSION}" VERSION_LESS "3.11" AND POLICY CMP0037)
	set_policy(CMP0037 OLD)
endif()

set(GTEST_VERSION 1.8.1)

find_package(GTest ${GTEST_VERSION})

if(NOT GTEST_FOUND)
	include(InstallGTest)

	if(NOT GTEST_FOUND)
		message(STATUS "GTest libraries not found")
		return()
	endif()

	set(GTEST_INSTALL TRUE)
endif()

# Create interface library to link against Google Test
add_library(GTest INTERFACE)

target_include_directories(GTest
	SYSTEM INTERFACE
	${GTEST_INCLUDE_DIRS}
)

target_link_libraries(GTest
	INTERFACE
	${GTEST_LIBRARIES}
)

if(GTEST_INSTALL)
	add_dependencies(GTest google-test-depends)
endif()

# Set memory check configuration
option(OPTION_TEST_MEMORYCHECK "Run tests with memory checker (valgrind)." OFF)

if(OPTION_TEST_MEMORYCHECK)
	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full")
	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes")
	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --show-reachable=yes")
	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --num-callers=50")

	set(MEMORYCHECK_SUPPRESSIONS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/memcheck/valgrind.supp")

	# TODO: Memory check does not work properly with CoreCLR
	#
	# 	Remove MEMCHECK_IGNORE label from the following tests:
	#		- cs-loader-test
	#		- metacall-test
	#		- metacall-distributable-test
	#		- metacall-inspect-test
	#		- metacall-integration-test

	add_custom_target(memcheck
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND ${CMAKE_CTEST_COMMAND}
			--label-exclude MEMCHECK_IGNORE
			--force-new-ctest-process
			--test-action memcheck
			--timeout 60
		#COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
	)
endif()

#
# Tests
#

include(CTest)

add_subdirectory(preprocessor_test)
add_subdirectory(environment_test)
add_subdirectory(log_test)
add_subdirectory(adt_set_test)
add_subdirectory(adt_trie_test)
add_subdirectory(reflect_value_cast_test)
add_subdirectory(reflect_function_test)
add_subdirectory(reflect_scope_test)
add_subdirectory(reflect_metadata_test)
add_subdirectory(dynlink_test)
add_subdirectory(detour_test)
add_subdirectory(serial_test)
add_subdirectory(configuration_test)
add_subdirectory(py_loader_test)
add_subdirectory(py_django_integration_test)
add_subdirectory(py_loader_port_test)
add_subdirectory(rb_loader_test)
add_subdirectory(rb_loader_parser_test)
add_subdirectory(rb_loader_parser_integration_test)
#add_subdirectory(rb_rails_integration_test) # TODO
add_subdirectory(cs_loader_test)
add_subdirectory(node_loader_test)
add_subdirectory(file_loader_test)
add_subdirectory(loader_path_test)
add_subdirectory(metacall_load_memory_test)
add_subdirectory(metacall_load_configuration_test)
add_subdirectory(metacall_load_configuration_relative_test)
add_subdirectory(metacall_duplicated_handle_test)
add_subdirectory(metacall_test)
add_subdirectory(metacall_node_test)
add_subdirectory(metacall_node_call_test)
add_subdirectory(metacall_distributable_test)
add_subdirectory(metacall_cast_test)
add_subdirectory(metacall_init_fini_test)
add_subdirectory(metacall_ducktype_test)
add_subdirectory(metacall_inspect_test)
add_subdirectory(metacall_integration_test)
add_subdirectory(metacall_depends_test)
add_subdirectory(metacall_configuration_exec_path_test)
add_subdirectory(metacall_clear_test)
add_subdirectory(metacall_python_gc_test)
add_subdirectory(metacall_map_test)
add_subdirectory(metacall_python_class_test)
add_subdirectory(metacall_initialize_test)
add_subdirectory(metacall_initialize_ex_test)
add_subdirectory(metacall_reinitialize_test)
add_subdirectory(metacall_fork_test)
