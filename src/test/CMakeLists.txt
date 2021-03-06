set(
    SHARED_SOURCES
    base_test.hpp
    testing_assert.cpp
    testing_assert.hpp
    sql/sqlite_testrunner/sqlite_wrapper.cpp
    sql/sqlite_testrunner/sqlite_wrapper.hpp
)

set(
    HYRISE_UNIT_TEST_SOURCES
    ${SHARED_SOURCES}
    concurrency/commit_context_test.cpp
    concurrency/transaction_context_test.cpp
    import_export/csv_meta_test.cpp
    lib/all_parameter_variant_test.cpp
    lib/all_type_variant_test.cpp
    operators/aggregate_test.cpp
    operators/delete_test.cpp
    operators/difference_test.cpp
    operators/export_binary_test.cpp
    operators/export_csv_test.cpp
    operators/get_table_test.cpp
    operators/import_binary_test.cpp
    operators/import_csv_test.cpp
    operators/index_scan_test.cpp
    operators/insert_test.cpp
    operators/join_equi_test.cpp
    operators/join_full_test.cpp
    operators/join_null_test.cpp
    operators/join_semi_anti_test.cpp
    operators/join_test.hpp
    operators/limit_test.cpp
    operators/print_test.cpp
    operators/product_test.cpp
    operators/projection_test.cpp
    operators/recreation_test.cpp
    operators/sort_test.cpp
    operators/table_scan_like_test.cpp
    operators/table_scan_test.cpp
    operators/union_all_test.cpp
    operators/union_positions_test.cpp
    operators/update_test.cpp
    operators/validate_test.cpp
    operators/validate_visibility_test.cpp
    logical_query_plan/logical_query_plan_test.cpp
    logical_query_plan/aggregate_node_test.cpp
    logical_query_plan/delete_node_test.cpp
    logical_query_plan/dummy_table_node_test.cpp
    logical_query_plan/insert_node_test.cpp
    logical_query_plan/join_node_test.cpp
    logical_query_plan/limit_node_test.cpp
    logical_query_plan/mock_node_test.cpp
    logical_query_plan/stored_table_node_test.cpp
    logical_query_plan/predicate_node_test.cpp
    logical_query_plan/projection_node_test.cpp
    logical_query_plan/show_columns_node_test.cpp
    logical_query_plan/show_tables_node_test.cpp
    logical_query_plan/sort_node_test.cpp
    logical_query_plan/union_node_test.cpp
    logical_query_plan/update_node_test.cpp
    logical_query_plan/validate_node_test.cpp
    optimizer/ast_to_operator_translator_test.cpp
    optimizer/column_statistics_test.cpp
    optimizer/strategy/join_detection_rule_test.cpp
    optimizer/expression_test.cpp
    optimizer/strategy/predicate_reordering_test.cpp
    optimizer/strategy/strategy_base_test.cpp
    optimizer/table_statistics_join_test.cpp
    optimizer/table_statistics_test.cpp
    scheduler/scheduler_test.cpp
    sql/sql_base_test.cpp
    sql/sql_basic_cache_test.cpp
    sql/sql_expression_translator_test.cpp
    sql/sql_parse_tree_cache_test.cpp
    sql/sql_prepare_execute_test.cpp
    sql/sql_query_operator_test.cpp
    sql/sql_query_plan_test.cpp
    sql/sql_query_plan_cache_test.cpp
    sql/sql_translator_test.cpp
    sql/sqlite_testrunner/sqlite_testrunner.cpp
    storage/adaptive_radix_tree_index_test.cpp
    storage/chunk_test.cpp
    storage/composite_group_key_index_test.cpp
    storage/dictionary_column_test.cpp
    storage/group_key_index_test.cpp
    storage/iterables_test.cpp
    storage/multi_column_index_test.cpp
    storage/numa_placement_test.cpp
    storage/reference_column_test.cpp
    storage/single_column_index_test.cpp
    storage/storage_manager_test.cpp
    storage/table_test.cpp
    storage/value_column_test.cpp
    storage/variable_length_key_base_test.cpp
    storage/variable_length_key_store_test.cpp
    storage/variable_length_key_test.cpp
    tasks/chunk_compression_task_test.cpp
    tasks/operator_task_test.cpp
    utils/cuckoo_hashtable_test.cpp
    utils/numa_memory_resource_test.cpp
    gtest_main.cpp
)

set (
    SYSTEM_TEST_SOURCES
    ${SHARED_SOURCES}
    tpc/tpcc_ref_test.cpp
    tpc/tpch_test.cpp
    tpc/tpch_db_generator_test.cpp
    gtest_main.cpp
)


# Both hyriseTest and hyriseSystemTest link against these
set(
    LIBRARIES
    gtest
    sqlite3
)

# Build special sanitizer version of googletest
include_directories(../../third_party/googletest/googletest/)

set(
    GTEST_SOURCES
    ../../third_party/googletest/googletest/src/gtest-all.cc
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Configure hyriseTest
add_executable(hyriseTest ${HYRISE_UNIT_TEST_SOURCES})
target_link_libraries(hyriseTest hyrise hyriseBenchmarkLib ${LIBRARIES})

# Configure hyriseSystemTest
add_executable(hyriseSystemTest ${SYSTEM_TEST_SOURCES})
target_link_libraries(hyriseSystemTest hyrise hyriseBenchmarkLib ${LIBRARIES})
add_dependencies(hyriseSystemTest tpccTableGenerator)

