#include <gtest/gtest.h>
#include <cereal_pack.hpp>
#include <cereal_pack_test/test/SimpleTest.hpp>
#include <cereal_pack_test/test/nesting/Nesting.hpp>
#include <OneBool.hpp>

using namespace cereal_pack_test::test;
using namespace cereal_pack_test::test::nesting;
using namespace cereal_pack;

class ConstantsTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(ConstantsTest, Constants) {
    EXPECT_EQ(1, OneBool::constants::max_serial_length);

    EXPECT_EQ(32, SimpleTest::constants::string_max_length);
    EXPECT_EQ(32, SimpleTest::constants::dynamic_length_buffer_max_length);
    EXPECT_EQ(16, SimpleTest::constants::const_length_buffer_max_length);
    EXPECT_EQ(5, SimpleTest::constants::set_of_bools_max_items);
    EXPECT_EQ(5, SimpleTest::constants::set_of_references_max_items);
    EXPECT_EQ(5, SimpleTest::constants::set_of_buffers_max_items);
    EXPECT_EQ(24, SimpleTest::constants::set_of_buffers_item_max_length);
    EXPECT_EQ(259, SimpleTest::constants::max_serial_length);

    EXPECT_EQ(5, Nesting::constants::set_of_bool_max_items);
    EXPECT_EQ(5, Nesting::constants::set_of_simple_max_items);
    EXPECT_EQ(1568, Nesting::constants::max_serial_length);
};
