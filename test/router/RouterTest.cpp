#include <gtest/gtest.h>
#include <cereal_pack/cereal_pack.hpp>
#include <cereal_pack/router/Router.hpp>
#include <Header.hpp>
#include <TypeOne.hpp>
#include <TypeTwo.hpp>
#include <TypeThree.hpp>

using namespace cereal_pack;
using namespace cereal_pack::router;

std::array<uint8_t, 32> fake_data;

class RouterTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::fill(fake_data.begin(), fake_data.end(), 0xEF);
    }

    void TearDown() override {
    }

    template<class T>
    std::vector<uint8_t> make_message(const T& body) {
        Header header;
        header.body_length().set(body.serial_length());
        header.name().set(body.schema_name());

        std::vector<uint8_t> msg;
        msg.resize(header.serial_length() + body.serial_length());
        auto l = header.serialize(msg.data());
        body.serialize(msg.data() + l);
        return msg;
    }
};

IngestedHeader ingest_header(Header h) {
    return router::IngestedHeader(
        h.serial_length(),
        h.name().get()
    );
};

TEST_F(RouterTest, BasicRouting) {
    Router<Header> router {ingest_header};
    bool called = false;
    bool attatched = router.attatch_route<TypeOne>([&called](const TypeOne& body) {
        EXPECT_EQ(body.key().get(), "yalla");
        EXPECT_EQ(0, memcmp(body.value().get(), fake_data.data(), 32));
        called = true;
    });
    ASSERT_TRUE(attatched);

    TypeOne body;
    body.key().set("yalla");
    body.value().set(fake_data.data());

    auto msg = make_message(body);

    ASSERT_TRUE(router.handle_message(msg.data()));

    EXPECT_TRUE(called);
}

TEST_F(RouterTest, RoutingWithMultipleRoutes) {
    Router<Header> router {ingest_header};
    bool typeOneCalled = false;
    bool attatched = router.attatch_route<TypeOne>([&typeOneCalled](const TypeOne& body) {
        EXPECT_EQ(body.key().get(), "yalla");
        EXPECT_EQ(0, memcmp(body.value().get(), fake_data.data(), 32));
        typeOneCalled = true;
    });
    ASSERT_TRUE(attatched);

    bool typeTwoCalled = false;
    attatched = router.attatch_route<TypeTwo>([&typeTwoCalled](const TypeTwo& body) {
        typeTwoCalled = true;
    });
    ASSERT_TRUE(attatched);

    bool typeThreeCalled = false;
    attatched = router.attatch_route<TypeThree>([&typeThreeCalled](const TypeThree& body) {
        EXPECT_EQ(body.pos_x().get(), 0);
        EXPECT_EQ(body.pos_y().get(), 283);
        EXPECT_EQ(body.pos_z().get(), 82103);
        EXPECT_EQ(body.scale_x().get(), 0.8);
        EXPECT_EQ(body.scale_y().get(), 3.14);
        EXPECT_EQ(body.scale_z().get(), 100.000001);
        typeThreeCalled = true;
    });
    ASSERT_TRUE(attatched);

    TypeOne oneBody;
    oneBody.key().set("yalla");
    oneBody.value().set(fake_data.data());
    auto msg = make_message(oneBody);
    ASSERT_TRUE(router.handle_message(msg.data()));
    EXPECT_TRUE(typeOneCalled);
    EXPECT_FALSE(typeTwoCalled);
    EXPECT_FALSE(typeThreeCalled);

    typeOneCalled = false;
    typeTwoCalled = false;
    typeThreeCalled = false;

    TypeThree threeBody;
    threeBody.pos_x().set(0);
    threeBody.pos_y().set(283);
    threeBody.pos_z().set(82103);
    threeBody.scale_x().set(0.8);
    threeBody.scale_y().set(3.14);
    threeBody.scale_z().set(100.000001);
    msg = make_message(threeBody);
    ASSERT_TRUE(router.handle_message(msg.data()));
    EXPECT_FALSE(typeOneCalled);
    EXPECT_FALSE(typeTwoCalled);
    EXPECT_TRUE(typeThreeCalled);
}

TEST_F(RouterTest, AttatchDuplicateRouteReturnsFalse) {
    Router<Header> router {ingest_header};
    EXPECT_TRUE(router.attatch_route<TypeOne>([](const TypeOne&) { }));
    EXPECT_FALSE(router.attatch_route<TypeOne>([](const TypeOne&) { }));
}

TEST_F(RouterTest, UnhandledMessageReturnsFalse) {
    Router<Header> router {ingest_header};
    TypeOne oneBody;
    auto msg = make_message(oneBody);
    EXPECT_FALSE(router.handle_message(msg.data()));
}
