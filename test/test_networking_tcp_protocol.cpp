#include <gtest/gtest.h>

#include <networking/tcp/protocol.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>

#include <vector>

TEST(networking_tcp_protocol, payload_encoding) {

    payload wr;
    wr.key = "mom-core/threshold";
    wr.type = MESSAGE_TYPE_DOUBLE;

    std::stringstream ss;
    cereal::JSONOutputArchive ar(ss);

    ar( wr );

    std::cout << ss.str() << std::endl;

    ASSERT_EQ(1,1);

}

TEST(networking_tcp_protocol, payload_decoding) {

    payload wr;
    wr.key = "mom-core/threshold";
    wr.type = MESSAGE_TYPE_DOUBLE;

    std::stringstream ss;
    cereal::JSONOutputArchive ar(ss);

    ar( wr );

    std::cout << ss.str() << std::endl;

    ASSERT_EQ(1,1);

}