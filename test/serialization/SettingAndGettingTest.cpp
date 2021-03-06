#include <gtest/gtest.h>
#include <OneBool.hpp>
#include <crunchy_bytes/crunchy_bytes.hpp>
#include <crunchy_bytes_test/test/SimpleTest.hpp>
#include <crunchy_bytes_test/test/nesting/Nesting.hpp>
#include <UsingGlobals.hpp>
#include <EnumExample.hpp>

class SettingAndGettingTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(SettingAndGettingTest, CanConstructGeneratedClasses) {
    crunchy_bytes_test::test::SimpleTest s;
    OneBool b;
    crunchy_bytes_test::test::nesting::Nesting n;
}

TEST_F(SettingAndGettingTest, MaxLengthsMatch) {
    using namespace crunchy_bytes_test::test;
    SimpleTest s;

    EXPECT_EQ(s.max_serial_length(), SimpleTest::constants::max_serial_length);

    nesting::Nesting n;
    EXPECT_EQ(n.max_serial_length(), nesting::Nesting::constants::max_serial_length);
}

TEST_F(SettingAndGettingTest, CanSetBool) {
    crunchy_bytes_test::test::SimpleTest s;
    s.boolean().set(false);
    EXPECT_FALSE(s.boolean().get());
    s.boolean().set(true);
    EXPECT_TRUE(s.boolean().get());
}

TEST_F(SettingAndGettingTest, CanSetString) {
    crunchy_bytes_test::test::SimpleTest s;
    s.string().set("Wow it's a string");
    EXPECT_EQ(s.string().get(), "Wow it's a string");
    s.string().set("Wow it's still a string");
    EXPECT_EQ(s.string().get(), "Wow it's still a string");
}

TEST_F(SettingAndGettingTest, CanSetUint8) {
    crunchy_bytes_test::test::SimpleTest s;
    s.uint8().set(32);
    EXPECT_EQ(s.uint8().get(), 32);
    s.uint8().set(251);
    EXPECT_EQ(s.uint8().get(), 251);
}

TEST_F(SettingAndGettingTest, CanSetInt8) {
    crunchy_bytes_test::test::SimpleTest s;
    s.int8().set(32);
    EXPECT_EQ(s.int8().get(), 32);
    s.int8().set(-4);
    EXPECT_EQ(s.int8().get(), -4);
}

TEST_F(SettingAndGettingTest, CanSetUint16) {
    crunchy_bytes_test::test::SimpleTest s;
    s.uint16().set(1450);
    EXPECT_EQ(s.uint16().get(), 1450);
    s.uint16().set(251);
    EXPECT_EQ(s.uint16().get(), 251);
}

TEST_F(SettingAndGettingTest, CanSetInt16) {
    crunchy_bytes_test::test::SimpleTest s;
    s.int16().set(1450);
    EXPECT_EQ(s.int16().get(), 1450);
    s.int16().set(-4);
    EXPECT_EQ(s.int16().get(), -4);
}

TEST_F(SettingAndGettingTest, CanSetUint32) {
    crunchy_bytes_test::test::SimpleTest s;
    s.uint32().set(242424242);
    EXPECT_EQ(s.uint32().get(), 242424242);
    s.uint32().set(251);
    EXPECT_EQ(s.uint32().get(), 251);
}

TEST_F(SettingAndGettingTest, CanSetInt32) {
    crunchy_bytes_test::test::SimpleTest s;
    s.int32().set(242424242);
    EXPECT_EQ(s.int32().get(), 242424242);
    s.int32().set(-4);
    EXPECT_EQ(s.int32().get(), -4);
}


TEST_F(SettingAndGettingTest, CanSetUint64) {
    crunchy_bytes_test::test::SimpleTest s;
    s.uint64().set(32);
    EXPECT_EQ(s.uint64().get(), 32);
    s.uint64().set(324425555225252252);
    EXPECT_EQ(s.uint64().get(), 324425555225252252);
}

TEST_F(SettingAndGettingTest, CanSetInt64) {
    crunchy_bytes_test::test::SimpleTest s;
    s.int64().set(324425555225252252);
    EXPECT_EQ(s.int64().get(), 324425555225252252);
    s.int64().set(-4);
    EXPECT_EQ(s.int64().get(), -4);
}

TEST_F(SettingAndGettingTest, CanSetConstLengthBuffer) {
    using namespace crunchy_bytes_test::test;
    SimpleTest s;

    std::vector<uint8_t> buff;
    buff.resize(SimpleTest::constants::const_length_buffer_max_length);
    std::fill(buff.begin(), buff.end(), 0xFF);

    s.const_length_buffer().set(buff.data());
    EXPECT_EQ(0, memcmp(s.const_length_buffer().get(), buff.data(), SimpleTest::constants::const_length_buffer_max_length));

    std::fill(buff.begin(), buff.end(), 0xBA);
    s.const_length_buffer().set(buff.data());
    EXPECT_EQ(0, memcmp(s.const_length_buffer().get(), buff.data(), SimpleTest::constants::const_length_buffer_max_length));

    std::fill(buff.begin(), buff.end(), 0xDE);
    s.const_length_buffer().set(buff);
    EXPECT_EQ(0, memcmp(s.const_length_buffer().get(), buff.data(), SimpleTest::constants::const_length_buffer_max_length));

    std::array<uint8_t, SimpleTest::constants::const_length_buffer_max_length> buff2;
    memset(buff2.data(), 0xEF, buff2.size());
    s.const_length_buffer().set(buff2);
    EXPECT_EQ(0, memcmp(s.const_length_buffer().get(), buff2.data(), SimpleTest::constants::const_length_buffer_max_length));
}

TEST_F(SettingAndGettingTest, CanSetDynamicLengthBuffer) {
    using namespace crunchy_bytes_test::test;
    SimpleTest s;

    std::vector<uint8_t> buff;
    buff.resize(10);
    std::fill(buff.begin(), buff.end(), 0xFF);

    s.dynamic_length_buffer().set(buff.data(), buff.size());
    EXPECT_EQ(s.dynamic_length_buffer().get().size(), buff.size());
    EXPECT_EQ(0, memcmp(s.dynamic_length_buffer().get().data(), buff.data(), s.dynamic_length_buffer().get().size()));

    buff.resize(SimpleTest::constants::dynamic_length_buffer_max_length);
    std::fill(buff.begin(), buff.end(), 0xBA);
    s.dynamic_length_buffer().set(buff.data(), buff.size());
    EXPECT_EQ(s.dynamic_length_buffer().get().size(), buff.size());
    EXPECT_EQ(0, memcmp(s.dynamic_length_buffer().get().data(), buff.data(), s.dynamic_length_buffer().get().size()));

    s.dynamic_length_buffer().reset();
    EXPECT_EQ(0, s.dynamic_length_buffer().size());

    std::fill(buff.begin(), buff.end(), 0xAB);
    s.dynamic_length_buffer().set(buff);
    EXPECT_EQ(s.dynamic_length_buffer().get().size(), buff.size());
    EXPECT_EQ(0, memcmp(s.dynamic_length_buffer().get().data(), buff.data(), s.dynamic_length_buffer().get().size()));

    s.dynamic_length_buffer().reset();
    EXPECT_EQ(0, s.dynamic_length_buffer().size());

    std::array<uint8_t, 5> buff2;
    std::fill(buff2.begin(), buff2.end(), 0xCC);
    s.dynamic_length_buffer().set(buff2);
    EXPECT_EQ(s.dynamic_length_buffer().get().size(), buff2.size());
    EXPECT_EQ(0, memcmp(s.dynamic_length_buffer().get().data(), buff2.data(), s.dynamic_length_buffer().get().size()));
}

TEST_F(SettingAndGettingTest, CanSetReference) {
    using namespace crunchy_bytes_test::test;
    SimpleTest s;
    OneBool b;
    b.boolean().set(true);
    s.reference() = b;
    EXPECT_TRUE(s.reference().boolean().get());

    b.boolean().set(false);
    s.reference() = std::move(b);
    EXPECT_FALSE(s.reference().boolean().get());

    crunchy_bytes::Reference<OneBool> bRef;
    bRef.get().boolean().set(true);
    s.reference() = bRef;
    EXPECT_TRUE(s.reference().boolean().get());
}

TEST_F(SettingAndGettingTest, CanSetSetOfPrimitives) {
    using namespace crunchy_bytes_test::test;
    SimpleTest s;
    s.set_of_bools().set({false, true, true});
    EXPECT_EQ(3, s.set_of_bools().get().size());
    EXPECT_FALSE(s.set_of_bools()[0]);
    EXPECT_FALSE(s.set_of_bools().get()[0]);
    EXPECT_TRUE(s.set_of_bools()[1]);
    EXPECT_TRUE(s.set_of_bools()[1].get());
    EXPECT_TRUE(s.set_of_bools()[2]);
    EXPECT_TRUE(s.set_of_bools().get()[2]);

    s.set_of_bools()[2] = false;
    EXPECT_FALSE(s.set_of_bools()[2]);;

    s.set_of_bools().reset();
    ASSERT_EQ(0, s.set_of_bools().size());

    s.set_of_bools().push_back(true);
    crunchy_bytes::Primitive<bool> b;
    b.set(false);
    s.set_of_bools().push_back(b);
    s.set_of_bools().push_back(false);

    EXPECT_EQ(3, s.set_of_bools().size());
    EXPECT_TRUE(s.set_of_bools()[0]);
    EXPECT_FALSE(s.set_of_bools()[1]);
    EXPECT_FALSE(s.set_of_bools()[2]);

    s.set_of_bools().reset();
    ASSERT_EQ(0, s.set_of_bools().size());

    s.set_of_bools().emplace_back(true);
    crunchy_bytes::Primitive<bool> b2;
    b2.set(false);
    s.set_of_bools().emplace_back(b2);
    s.set_of_bools().emplace_back(false);

    EXPECT_EQ(3, s.set_of_bools().size());
    EXPECT_TRUE(s.set_of_bools()[0]);
    EXPECT_FALSE(s.set_of_bools()[1]);
    EXPECT_FALSE(s.set_of_bools()[2]);
}

TEST_F(SettingAndGettingTest, CanSetSetOfReferences) {
    using namespace crunchy_bytes_test::test;
    SimpleTest s;

    std::vector<OneBool> refs;
    refs.resize(3);
    refs[0].boolean().set(true);
    refs[1].boolean().set(false);
    refs[2].boolean().set(true);
    s.set_of_references().set(refs);

    EXPECT_EQ(3, s.set_of_references().get().size());
    EXPECT_TRUE(s.set_of_references()[0].boolean());
    EXPECT_FALSE(s.set_of_references()[1].boolean());
    EXPECT_TRUE(s.set_of_references()[2].boolean());

    std::vector<crunchy_bytes::Reference<OneBool>> refs2;
    refs2.resize(3);
    refs2[0].get().boolean().set(true);
    refs2[1].get().boolean().set(false);
    refs2[2].get().boolean().set(true);
    s.set_of_references().set(refs2);

    EXPECT_EQ(3, s.set_of_references().size());
    EXPECT_TRUE(s.set_of_references()[0].boolean());
    EXPECT_FALSE(s.set_of_references()[1].boolean());
    EXPECT_TRUE(s.set_of_references()[2].boolean());

    s.set_of_references().reset();
    ASSERT_EQ(0, s.set_of_references().size());

    crunchy_bytes::Reference<OneBool> b;
    b.get().boolean().set(false);
    s.set_of_references().push_back(b);

    OneBool b2;
    b2.boolean().set(true);
    s.set_of_references().push_back(b2);

    OneBool b3;
    b3.boolean().set(true);
    s.set_of_references().emplace_back(b3);

    EXPECT_EQ(3, s.set_of_references().size());
    EXPECT_FALSE(s.set_of_references()[0].boolean());
    EXPECT_TRUE(s.set_of_references()[1].boolean());
    EXPECT_TRUE(s.set_of_references()[2].boolean());

    s.set_of_references().reset();
    ASSERT_EQ(0, s.set_of_references().size());

    s.set_of_references().resize(3);
    ASSERT_EQ(3, s.set_of_references().size());

    s.set_of_references()[0].boolean().set(false);
    s.set_of_references()[1].boolean().set(true);
    s.set_of_references()[2].boolean().set(true);
    EXPECT_FALSE(s.set_of_references()[0].boolean());
    EXPECT_TRUE(s.set_of_references()[1].boolean());
    EXPECT_TRUE(s.set_of_references()[2].boolean());

    s.set_of_references().reset();
    ASSERT_EQ(0, s.set_of_references().size());

    OneBool oneBool;
    oneBool.boolean().set(true);
    s.set_of_references().resize(1);
    s.set_of_references()[0] = oneBool;
    EXPECT_TRUE(s.set_of_references()[0].boolean());

    crunchy_bytes::Reference<OneBool> oneBool2;
    oneBool2.get().boolean().set(false);
    s.set_of_references()[0] = oneBool2;
    EXPECT_FALSE(s.set_of_references()[0].boolean());
}

TEST_F(SettingAndGettingTest, CanSetSetOfBuffers) {
    using namespace crunchy_bytes_test::test;
    SimpleTest s;

    constexpr auto buffLen = SimpleTest::constants::set_of_buffers_item_max_length;
    std::vector<crunchy_bytes::ConstLengthBuffer<buffLen>> buffs;

    buffs.resize(3);
    memset(buffs[0].get(), 0xFF, buffLen);
    memset(buffs[1].get(), 0xEE, buffLen);
    memset(buffs[2].get(), 0xDD, buffLen);
    s.set_of_buffers().set(buffs);

    ASSERT_EQ(3, s.set_of_buffers().get().size());
    EXPECT_EQ(s.set_of_buffers()[0].size(), buffLen);
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[0].get(), buffs[0].get(), buffLen));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[1].get(), buffs[1].get(), buffLen));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[2].get(), buffs[2].get(), buffLen));


    s.set_of_buffers().reset();
    ASSERT_EQ(0, s.set_of_buffers().size());

    std::vector<std::array<uint8_t, buffLen>> buffs2;
    buffs2.resize(3);
    memset(buffs2[0].data(), 0x0F, buffLen);
    memset(buffs2[1].data(), 0x4E, buffLen);
    memset(buffs2[2].data(), 0x8D, buffLen);
    s.set_of_buffers().set(buffs2);

    ASSERT_EQ(3, s.set_of_buffers().get().size());
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[0].get(), buffs2[0].data(), buffLen));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[1].get(), buffs2[1].data(), buffLen));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[2].get(), buffs2[2].data(), buffLen));

    std::vector<std::array<uint8_t, buffLen - 3>> buffs3;
    buffs3.resize(3);
    memset(buffs3[0].data(), 0x7F, buffLen - 3);
    memset(buffs3[1].data(), 0x3E, buffLen - 3);
    memset(buffs3[2].data(), 0x4D, buffLen - 3);
    s.set_of_buffers().set(buffs3);

    ASSERT_EQ(3, s.set_of_buffers().get().size());
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[0].get(), buffs3[0].data(), buffLen - 3));

    EXPECT_EQ(0, *(s.set_of_buffers()[0].get() + buffLen - 3));
    EXPECT_EQ(0, *(s.set_of_buffers()[0].get() + buffLen - 2));
    EXPECT_EQ(0, *(s.set_of_buffers()[0].get() + buffLen - 1));

    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[1].get(), buffs3[1].data(), buffLen - 3));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[2].get(), buffs3[2].data(), buffLen - 3));

    s.set_of_buffers().reset();
    ASSERT_EQ(0, s.set_of_buffers().size());

    std::array<uint8_t, buffLen> b;
    memset(b.data(), 0x88, buffLen);
    s.set_of_buffers().push_back(b);

    std::array<uint8_t, buffLen> b2;
    memset(b2.data(), 0x99, buffLen);
    s.set_of_buffers().push_back(b2.data());

    std::array<uint8_t, buffLen> b3;
    std::array<uint8_t, buffLen> b3_copy;
    memset(b3.data(), 0x77, buffLen);
    memset(b3_copy.data(), 0x77, buffLen);
    s.set_of_buffers().emplace_back(b3);

    EXPECT_EQ(3, s.set_of_buffers().size());
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[0].get(), b.data(), buffLen));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[1].get(), b2.data(), buffLen));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[2].get(), b3_copy.data(), buffLen));

    s.set_of_buffers().reset();
    ASSERT_EQ(0, s.set_of_buffers().size());

    s.set_of_buffers().resize(3);
    ASSERT_EQ(3, s.set_of_buffers().size());


    std::array<uint8_t, buffLen> buff;
    memset(buff.data(), 0x88, buffLen);
    s.set_of_buffers()[0].set(buff);

    std::array<uint8_t, buffLen> buff2;
    memset(buff2.data(), 0x78, buffLen);
    memset(s.set_of_buffers()[1].get(), 0x78, buffLen);
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[0].get(), buff.data(), buffLen));
    EXPECT_TRUE(0 == memcmp(s.set_of_buffers()[1].get(), buff2.data(), buffLen));
}

TEST_F(SettingAndGettingTest, SetNestedProperties) {
    using namespace crunchy_bytes_test::test::nesting;
    Nesting n;
    n.simple_ref().string().set("Yeah");
    EXPECT_EQ("Yeah", n.simple_ref().string().get());

    n.simple_ref().reference().boolean().set(true);
    EXPECT_TRUE(n.simple_ref().reference().boolean().get());

    n.simple_ref().reference().boolean() = {};
    EXPECT_FALSE(n.simple_ref().reference().boolean().get());

    n.simple_ref().reference().boolean() = {true};
    EXPECT_TRUE(n.simple_ref().reference().boolean().get());

    n.simple_ref().reference().reset();
    EXPECT_FALSE(n.simple_ref().reference().boolean().get());

    OneBool ob;
    ob.boolean().set(true);
    n.simple_ref().reference() = ob;
    EXPECT_TRUE(n.simple_ref().reference().boolean().get());

    ob.boolean().set(false);
    n.simple_ref().reference() = std::move(ob);
    EXPECT_FALSE(n.simple_ref().reference().boolean().get());

    EXPECT_FALSE(n.simple_ref().reference().boolean().get());
}

TEST_F(SettingAndGettingTest, SetPropetiesUsingGlobals) {
    UsingGlobals g;
    using namespace crunchy_bytes::globals;
    uint8_t buff[max_crunchy_bytes_serial_length];
    memset(buff, 0x89, sizeof(buff));
    g.data_one().set(buff);
    EXPECT_EQ(0, memcmp(g.data_one().get(), buff, max_item_length));

    g.data_two().set(buff, max_item_length);
    EXPECT_EQ(g.data_two().get().size(), max_item_length);
    EXPECT_EQ(0, memcmp(g.data_two().get().data(), buff, max_item_length));

    char name[max_name_length + 1] = {0};
    memset(name, 'z', max_name_length);
    g.name().set(std::string(name));
    EXPECT_EQ(std::string(name), g.name().get());

    std::vector<uint32_t> numbers;
    for (unsigned int i = 0; i < max_elements; i++) {
        numbers.push_back(rand());
    }
    g.some_list_of_numbers().set(numbers);
    for (int i = 0; i < max_elements; i++) {
        EXPECT_EQ(numbers[i], g.some_list_of_numbers()[i]);
    }

    for (unsigned int i = 0; i < max_elements; i++) {
        g.some_list_of_names().push_back(name);
    }
    for (unsigned int i = 0; i < max_elements; i++) {
        EXPECT_EQ(g.some_list_of_names()[i], std::string(name));
    }

    std::array<std::array<uint8_t, max_item_length>, max_elements> buffers;
    for (uint8_t i = 0; i < max_elements; i++) {
        std::fill(buffers[i].begin(), buffers[i].end(), i);
    }
    g.some_list_of_buffers().set(buffers);
    for (uint8_t i = 0; i < max_elements; i++) {
        EXPECT_EQ(0, memcmp(g.some_list_of_buffers()[i].get(), buffers[i].data(), max_item_length));
    }

    g.current_location().set(crunchy_bytes::globals::continent::asia);
    EXPECT_EQ(crunchy_bytes::globals::continent::asia, g.current_location().get());

    for (unsigned int i = 0; i < UsingGlobals::constants::bucket_list_max_items; i++) {
        g.bucket_list().push_back((crunchy_bytes::globals::continent)i);
    }
    for (unsigned int i = 0; i < UsingGlobals::constants::bucket_list_max_items; i++) {
        EXPECT_EQ((crunchy_bytes::globals::continent)i, g.bucket_list().get()[i]);
    }

    EXPECT_EQ(g.serial_length(), g.max_serial_length());
}

TEST_F(SettingAndGettingTest, SetEnums) {
    EnumExample e;
    e.simple_enum().set(EnumExample::numbers::one);
    EXPECT_EQ(EnumExample::numbers::one, e.simple_enum().get());
    e.another_enum().set(EnumExample::numbers::two);
    EXPECT_EQ(EnumExample::numbers::two, e.another_enum().get());

    e.set_of_enums().push_back(EnumExample::state::undefined);
    e.set_of_enums().push_back(EnumExample::state::working);
    e.set_of_enums().push_back(EnumExample::state::complete);
    EXPECT_EQ(EnumExample::state::undefined, e.set_of_enums().get()[0]);
    EXPECT_EQ(EnumExample::state::working, e.set_of_enums().get()[1]);
    EXPECT_EQ(EnumExample::state::complete, e.set_of_enums().get()[2]);
}
