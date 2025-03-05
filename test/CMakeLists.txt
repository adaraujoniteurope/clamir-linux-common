add_executable(
  packet_test
  packet_test.cpp
)

target_link_libraries(
  packet_test
  ${PROJECT_NAME}-obj
  GTest::gtest_main
)

gtest_discover_tests(packet_test)

add_executable(
  test_math_control
  test_math_control.cpp
)

target_link_libraries(
  test_math_control
  ${PROJECT_NAME}-obj
  GTest::gtest_main
)

gtest_discover_tests(test_math_control)