add_executable(parse_test parse_tests.c)
target_link_libraries(parse_test osup)

add_executable(bm_test bm_test.c)
target_link_libraries(bm_test osup)

