#include <gtest/gtest.h>

#include <networking/tcp/protocol_legacy.hpp>

class packet_test : public testing::Test {
    protected:

    packet_test() {
     }
   
     ~packet_test() override {
     }

     void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
     }
   
     void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
     }


};

// TEST_F(packet_test, packet_test_parse_endpoint) {
//     for (uint32_t i = 0; i < std::numeric_limits<uint32_t>::max(); i++) {
//         packet p = packet::parse(i);
//         ASSERT_TRUE(p.endpoint == ((i & 0x0000007F) >> 0));
//     }
// }

// TEST_F(packet_test, packet_test_encode_endpoint) {

//     packet p = packet::parse(0x00000000);
//     for (uint32_t i = 0; i < 0x7F; i++) {
//         p.endpoint = i;
//         auto data = packet::encode(p);
//         ASSERT_TRUE(p.endpoint == ((data & 0x0000007F) >> 0));
//     }
// }

// TEST_F(packet_test, packet_test_parse_rw) {
//     for (uint32_t i = 0; i < std::numeric_limits<uint32_t>::max(); i++) {
//         packet p = packet::parse(i);
//         if ((i & 0x00000080) > 0) {
//             ASSERT_TRUE(p.rw == 1);
//         } else {
//             ASSERT_TRUE(p.rw == 0);
//         }
//     }
// }

TEST_F(packet_test, packet_test_parse_group) {
    for (uint32_t i = 0; i < std::numeric_limits<uint32_t>::max(); i++) {
        packet p = packet::parse(i);
        ASSERT_TRUE(p.group == (i & 0x00000F00) >> 8);
    }
}

TEST_F(packet_test, packet_test_parse_value) {
    for (uint32_t i = 0; i < std::numeric_limits<uint32_t>::max(); i++) {
        packet p = packet::parse(i);
        ASSERT_TRUE(p.value == (i & 0xFFFF0000) >> 16);
    }
}