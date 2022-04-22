#include <gtest/gtest.h>
#include <cereal_pack.hpp>
#include <cereal_pack_test/test/SimpleTest.hpp>
#include <cereal_pack_test/test/nesting/Nesting.hpp>
#include <OneBool.hpp>
#include <UsingGlobals.hpp>

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
    EXPECT_EQ(271, SimpleTest::constants::max_serial_length);

    EXPECT_EQ(5, Nesting::constants::set_of_bool_max_items);
    EXPECT_EQ(5, Nesting::constants::set_of_simple_max_items);
    EXPECT_EQ(1640, Nesting::constants::max_serial_length);
};

TEST_F(ConstantsTest, GlobalConstants) {
    EXPECT_EQ(4096, cereal_pack::globals::max_cereal_pack_serial_length);

    EXPECT_EQ(10, cereal_pack::globals::max_elements);
    EXPECT_EQ(100, cereal_pack::globals::max_item_length);
    EXPECT_EQ(255, cereal_pack::globals::max_name_length);

    EXPECT_EQ(cereal_pack::globals::max_item_length, UsingGlobals::constants::data_one_max_length);
    EXPECT_EQ(cereal_pack::globals::max_item_length, UsingGlobals::constants::data_two_max_length);
    EXPECT_EQ(cereal_pack::globals::max_name_length, UsingGlobals::constants::name_max_length);
    EXPECT_EQ(cereal_pack::globals::max_name_length, UsingGlobals::constants::some_list_of_names_item_max_length);
    EXPECT_EQ(cereal_pack::globals::max_item_length, UsingGlobals::constants::some_list_of_buffers_item_max_length);

    EXPECT_EQ(cereal_pack::globals::max_elements, UsingGlobals::constants::some_list_of_numbers_max_items);
    EXPECT_EQ(cereal_pack::globals::max_elements, UsingGlobals::constants::some_list_of_names_max_items);
    EXPECT_EQ(cereal_pack::globals::max_elements, UsingGlobals::constants::some_list_of_buffers_max_items);
}
