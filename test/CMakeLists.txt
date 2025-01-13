add_executable(
  test_networking_tcp_protocol
  test_networking_tcp_protocol.cpp
)

target_link_libraries(
  test_networking_tcp_protocol
  ${PROJECT_NAME}-obj
  GTest::gtest_main
)

gtest_discover_tests(test_networking_tcp_protocol)