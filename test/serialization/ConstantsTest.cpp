#include <gtest/gtest.h>
#include <crunchy_bytes/crunchy_bytes.hpp>
#include <crunchy_bytes_test/test/SimpleTest.hpp>
#include <crunchy_bytes_test/test/nesting/Nesting.hpp>
#include <OneBool.hpp>
#include <UsingGlobals.hpp>

using namespace crunchy_bytes_test::test;
using namespace crunchy_bytes_test::test::nesting;
using namespace crunchy_bytes;

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
    EXPECT_EQ(8192, crunchy_bytes::globals::max_crunchy_bytes_serial_length);

    EXPECT_EQ(10, crunchy_bytes::globals::max_elements);
    EXPECT_EQ(100, crunchy_bytes::globals::max_item_length);
    EXPECT_EQ(255, crunchy_bytes::globals::max_name_length);

    EXPECT_EQ(crunchy_bytes::globals::max_item_length, UsingGlobals::constants::data_one_max_length);
    EXPECT_EQ(crunchy_bytes::globals::max_item_length, UsingGlobals::constants::data_two_max_length);
    EXPECT_EQ(crunchy_bytes::globals::max_name_length, UsingGlobals::constants::name_max_length);
    EXPECT_EQ(crunchy_bytes::globals::max_name_length, UsingGlobals::constants::some_list_of_names_item_max_length);
    EXPECT_EQ(crunchy_bytes::globals::max_item_length, UsingGlobals::constants::some_list_of_buffers_item_max_length);

    EXPECT_EQ(crunchy_bytes::globals::max_elements, UsingGlobals::constants::some_list_of_numbers_max_items);
    EXPECT_EQ(crunchy_bytes::globals::max_elements, UsingGlobals::constants::some_list_of_names_max_items);
    EXPECT_EQ(crunchy_bytes::globals::max_elements, UsingGlobals::constants::some_list_of_buffers_max_items);
}

TEST_F(ConstantsTest, SchemaNames) {
    EXPECT_EQ(0, strcmp("crunchy_bytes_test::test::SimpleTest", SimpleTest::constants::schema_name));
    SimpleTest s;
    EXPECT_EQ(0, strcmp("crunchy_bytes_test::test::SimpleTest", s.schema_name()));

    EXPECT_EQ(0, strcmp("crunchy_bytes_test::test::nesting::Nesting", Nesting::constants::schema_name));
    Nesting n;
    EXPECT_EQ(0, strcmp("crunchy_bytes_test::test::nesting::Nesting", n.schema_name()));

    EXPECT_EQ(0, strcmp("OneBool", OneBool::constants::schema_name));
    OneBool ob;
    EXPECT_EQ(0, strcmp("OneBool", ob.schema_name()));
}
