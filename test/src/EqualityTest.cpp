#include <gtest/gtest.h>
#include <cereal_pack.hpp>
#include <cereal_pack_test/test/SimpleTest.hpp>
#include <cereal_pack_test/test/nesting/Nesting.hpp>
#include <OneBool.hpp>

using namespace cereal_pack_test::test;
using namespace cereal_pack_test::test::nesting;
class EqualityTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    void PopulateSimpleTest(SimpleTest& st) {
        uint8_t buff[100];
        memset(buff, 0xcc, sizeof(buff));

        OneBool obTrue;
        obTrue.boolean().set(true);
        OneBool obFalse;
        obFalse.boolean().set(false);

        st.string().set("Yo");
        st.boolean().set(true);
        st.uint8().set(22);
        st.int8().set(-22);

        st.uint16().set(288);
        st.int16().set(-288);

        st.uint32().set(22231);
        st.int32().set(-22231);

        st.uint64().set(22231214);
        st.int64().set(-22231214);

        st.dynamic_length_buffer().set(buff, SimpleTest::constants::dynamic_length_buffer_max_length);
        st.const_length_buffer().set(buff);

        st.reference().boolean().set(true);

        st.set_of_bools().push_back(true);
        st.set_of_bools().push_back(false);
        st.set_of_bools().push_back(true);

        st.set_of_references().push_back(obTrue);
        st.set_of_references().push_back(obFalse);
        st.set_of_references().push_back(obTrue);

        st.set_of_buffers().resize(3);
        st.set_of_buffers()[0].set(buff);
        st.set_of_buffers()[1].set(buff);
        st.set_of_buffers()[2].set(buff);
    }
};

TEST_F(EqualityTest, EqualityOneBool) {
    OneBool ob1;
    ob1.boolean().set(true);

    OneBool ob2;
    ob2.boolean().set(true);

    ASSERT_EQ(ob2, ob1);

    ob2.boolean().set(false);
    ASSERT_NE(ob2, ob1);

    ob1.boolean().set(false);
    ASSERT_EQ(ob2, ob1);
}

TEST_F(EqualityTest, EqualitySimple) {
    SimpleTest st1;
    PopulateSimpleTest(st1);
    SimpleTest st2;
    PopulateSimpleTest(st2);

    ASSERT_EQ(st2, st1);

    st2.boolean().set(false);
    ASSERT_NE(st2, st1);

    st1.boolean().set(false);
    ASSERT_EQ(st2, st1);

    st1.set_of_references().resize(1);
    ASSERT_NE(st2, st1);
    st2.set_of_references().resize(1);
    ASSERT_EQ(st2, st1);

    st1.dynamic_length_buffer().reset();
    ASSERT_NE(st2, st1);
    st2.dynamic_length_buffer().reset();
    ASSERT_EQ(st2, st1);
}

TEST_F(EqualityTest, EqualityNested) {
    Nesting n1;
    n1.bool_ref().boolean().set(true);
    PopulateSimpleTest(n1.simple_ref());

    n1.set_of_bool().resize(5);
    n1.set_of_bool()[1].boolean().set(true);
    n1.set_of_bool()[3].boolean().set(true);

    n1.set_of_simple().resize(5);
    PopulateSimpleTest(n1.set_of_simple()[1]);
    PopulateSimpleTest(n1.set_of_simple()[2]);

    Nesting n2;
    n2.bool_ref().boolean().set(true);
    PopulateSimpleTest(n2.simple_ref());

    n2.set_of_bool().resize(5);
    n2.set_of_bool()[1].boolean().set(true);
    n2.set_of_bool()[3].boolean().set(true);

    n2.set_of_simple().resize(5);
    PopulateSimpleTest(n2.set_of_simple()[1]);
    PopulateSimpleTest(n2.set_of_simple()[2]);

    ASSERT_EQ(n1, n2);

    n1.bool_ref().boolean().set(false);
    ASSERT_NE(n1, n2);

    n2.bool_ref().boolean().set(false);
    ASSERT_EQ(n1, n2);

    n1.set_of_simple().resize(1);
    ASSERT_NE(n1, n2);
    n2.set_of_simple().resize(1);
    ASSERT_EQ(n1, n2);
}
