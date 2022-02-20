#!/bin/bash
# ROOT path is the absolute path to your project root directory
ROOT=""
# Test PATH is from autotester exec
TEST_PATH=""
cd $ROOT
# Runs Quiz test case
./autotester $TEST_PATH/quiz_test_cases/Single_nested_if_while_statements_source.txt $TEST_PATH/quiz_test_cases/Single_nested_if_while_statements_queries.txt $TEST_PATH/output/Single_nested_if_while_statements.xml
./autotester $TEST_PATH/quiz_test_cases/Single_procedure_without_nested_statements_source.txt $TEST_PATH/quiz_test_cases/Single_procedure_without_nested_statements_queries.txt $TEST_PATH/output/Single_procedure_without_nested_statements_queries.xml

# Runs Wiki test case
./autotester $TEST_PATH/wiki/wiki_code_5_source.txt $TEST_PATH/wiki/wiki_code_5_queries.txt $TEST_PATH/output/out5.xml
./autotester $TEST_PATH/wiki/wiki_code_6_source.txt $TEST_PATH/wiki/wiki_code_6_queries.txt $TEST_PATH/output/out6.xml
./autotester $TEST_PATH/wiki/wiki_code_8_source.txt $TEST_PATH/wiki/wiki_code_8_queries.txt $TEST_PATH/output/out8.xml
./autotester $TEST_PATH/wiki/wiki_code_9_source.txt $TEST_PATH/wiki/wiki_code_9_queries.txt $TEST_PATH/output/out9.xml
./autotester $TEST_PATH/wiki/wiki_code_10_source.txt $TEST_PATH/wiki/wiki_code_10_queries.txt $TEST_PATH/output/out10.xml

# Runs extended Wiki test case
./autotester $TEST_PATH/wiki/wiki_code_5_source_extended.txt $TEST_PATH/wiki/wiki_code_5_queries_extended.txt $TEST_PATH/output/out5_extended.xml
./autotester $TEST_PATH/wiki/wiki_code_8_source_extended.txt $TEST_PATH/wiki/wiki_code_8_queries_extended.txt $TEST_PATH/output/out8_extended.xml
./autotester $TEST_PATH/wiki/wiki_code_9_source_extended.txt $TEST_PATH/wiki/wiki_code_9_queries_extended.txt $TEST_PATH/output/out9_extended.xml
./autotester $TEST_PATH/wiki/wiki_code_10_source_extended.txt $TEST_PATH/wiki/wiki_code_10_queries_extended.txt $TEST_PATH/output/out10_extended.xml

# Runs Custom Test Case
./autotester $TEST_PATH/custom_test_cases/complex_cfg_source.txt $TEST_PATH/custom_test_cases/complex_cfg_queries.txt $TEST_PATH/output/complex_cfg.xml
./autotester $TEST_PATH/custom_test_cases/multiple_procedure_nested_calls_source.txt $TEST_PATH/custom_test_cases/multiple_procedure_nested_calls_queries.txt $TEST_PATH/output/multiple_procedure_nested_calls.xml
./autotester $TEST_PATH/custom_test_cases/single_procedure_duplicable_source.txt $TEST_PATH/custom_test_cases/single_procedure_duplicable_queries.txt $TEST_PATH/output/single_procedure_duplicable_queries.xml
./autotester $TEST_PATH/custom_test_cases/single_procedure_nested_if_while_SIMPLE_source.txt $TEST_PATH/custom_test_cases/single_procedure_nested_if_while_SIMPLE_queries.txt $TEST_PATH/output/single_procedure_nested_if_while_SIMPLE.xml

