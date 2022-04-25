#include <gtest/gtest.h>
#include <cereal_pack/cereal_pack.hpp>
#include <cereal_pack/router/Router.hpp>
#include <EnumHeader.hpp>
#include <TypeTwo.hpp>
#include <TypeTwo.hpp>
#include <TypeThree.hpp>

using namespace cereal_pack;
using namespace cereal_pack::router;

class EnumRouterTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    template<class T>
    std::vector<uint8_t> make_message(EnumHeader::body_t type, const T& body) {
        EnumHeader header;
        header.body_length().set(body.serial_length());
        header.body_type().set(type);

        std::vector<uint8_t> msg;
        msg.resize(header.serial_length() + body.serial_length());
        auto l = header.serialize(msg.data());
        body.serialize(msg.data() + l);
        return msg;
    }
};

EnumHeader::body_t ingest_header(const EnumHeader& h) {
    return h.body_type().get();
};


TEST_F(EnumRouterTest, BasicRouting) {
    Router<EnumHeader, EnumHeader::body_t> router ([](const EnumHeader& h) {
        return h.body_type().get();
    });
    bool called = false;
    bool attatched = router.attatch_route<TypeTwo>(EnumHeader::body_t::type_one, [&called](const TypeTwo& body) {
        EXPECT_EQ(body.color_hex().get(), "#FFAA88");
        EXPECT_EQ(body.pos_x().get(), 1841);
        EXPECT_EQ(body.pos_y().get(), 82);
        called = true;
    });
    ASSERT_TRUE(attatched);

    TypeTwo body;
    body.color_hex().set("#FFAA88");
    body.pos_x().set(1841);
    body.pos_y().set(82);

    auto msg = make_message(EnumHeader::body_t::type_one, body);

    ASSERT_TRUE(router.handle_message(msg.data()));

    EXPECT_TRUE(called);
}
