



add_executable(
  test_shared_memory
  test_shared_memory.cpp
)

target_link_libraries(
  test_shared_memory
  GTest::gtest_main
)

gtest_discover_tests(test_shared_memory)