#include <gtest/gtest.h>
#include <cereal_pack/cereal_pack.hpp>
#include <cereal_pack/router/Router.hpp>
#include <Header.hpp>
#include <TypeOne.hpp>
#include <TypeTwo.hpp>
#include <TypeThree.hpp>

using namespace cereal_pack;
using namespace cereal_pack::router;

class RouterTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

};

IngestedHeader ingest_header(Header h) {
    return router::IngestedHeader(
        h.serial_length(),
        h.body_length().get() + h.serial_length(),
        h.name().get()
    );
};

TEST_F(RouterTest, Routing) {
    std::array<uint8_t, 32> data;
    std::fill(data.begin(), data.end(), 0xEF);

    Router<Header> router {ingest_header};
    bool called = false;
    router.attatch_route<TypeOne>([&data, &called](const TypeOne& body) {
        EXPECT_EQ(body.key().get(), "yalla");
        EXPECT_EQ(0, memcmp(body.value().get(), data.data(), 32));
        called = true;
    });

    TypeOne body;
    body.key().set("yalla");
    body.value().set(data.data());

    Header header;
    header.body_length().set(body.serial_length());
    header.name().set(body.schema_name());

    std::vector<uint8_t> msg;
    msg.resize(header.serial_length() + body.serial_length());
    auto l = header.serialize(msg.data());
    body.serialize(msg.data() + l);

    router.handle_message(msg.data());

    EXPECT_TRUE(called);
}
