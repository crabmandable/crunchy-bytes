#include <gtest/gtest.h>
#include <cereal_pack/cereal_pack.hpp>

using namespace cereal_pack;

class ExceptionTest : public ::testing::Test {
    void SetUp() override {
    }
    void TearDown() override {
    }
};

TEST_F(ExceptionTest, ConstructingConstLengthBuffer) {
    std::vector<uint8_t> vec {1, 2, 3};
    EXPECT_THROW(ConstLengthBuffer<2> cb {vec} , CerealPackException);
}

TEST_F(ExceptionTest, ConstructingDynamicLengthBuffer) {
    std::vector<uint8_t> vec {1, 2, 3};
    EXPECT_THROW(DynamicLengthBuffer<2> db {vec} , CerealPackException);
}

TEST_F(ExceptionTest, DeserializingDynamicLengthBuffer) {
    std::vector<uint8_t> vec {1, 2, 3};
    DynamicLengthBuffer<3> db1 {vec};
    std::vector<uint8_t> buff;
    buff.resize(db1.serial_length());
    db1.serialize(buff.data());

    DynamicLengthBuffer<2> db2;
    EXPECT_THROW(db2.deserialize(buff.data()), CerealPackException);
}

TEST_F(ExceptionTest, SetDynamicLengthBufferFromPointer) {
    std::vector<uint8_t> vec;
    vec.resize(100);
    DynamicLengthBuffer<2> db;
    EXPECT_THROW(db.set(vec.data(), vec.size()), CerealPackException);
}

TEST_F(ExceptionTest, SetDynamicLengthBufferFromContainer) {
    std::vector<uint8_t> vec;
    vec.resize(100);
    DynamicLengthBuffer<2> db;
    EXPECT_THROW(db.set(vec), CerealPackException);
}

TEST_F(ExceptionTest, DeserializingSet) {
    Set<Primitive<uint32_t>, 2> s1;
    s1.push_back(10);
    s1.push_back(11);
    std::vector<uint8_t> buff;
    buff.resize(s1.serial_length());
    s1.serialize(buff.data());

    Set<Primitive<uint32_t>, 1> s2;
    EXPECT_THROW(s2.deserialize(buff.data()), CerealPackException);
}

TEST_F(ExceptionTest, SetSet) {
    Set<Primitive<uint32_t>, 2> s;
    std::vector<uint32_t> vec1 {1, 2, 3, 4};
    EXPECT_THROW(s.set(vec1), CerealPackException);
    std::vector<Primitive<uint32_t>> vec2 {1, 2, 3, 4};
    EXPECT_THROW(s.set(vec2), CerealPackException);
}

TEST_F(ExceptionTest, PushBackSet) {
    Set<Primitive<uint32_t>, 2> s;
    s.push_back(1);
    s.push_back(2);
    EXPECT_THROW(s.push_back(3), CerealPackException);
}

TEST_F(ExceptionTest, EmplaceBackSet) {
    Set<Primitive<uint32_t>, 2> s;
    s.emplace_back(1);
    s.emplace_back(2);
    EXPECT_THROW(s.emplace_back(3), CerealPackException);
}

TEST_F(ExceptionTest, ResizeSet) {
    Set<Primitive<uint32_t>, 2> s;
    EXPECT_THROW(s.resize(3), CerealPackException);
}

TEST_F(ExceptionTest, DeserializeString) {
    String<2> s;
    std::string str = "Hello";
    EXPECT_THROW(s.deserialize(str.c_str()), CerealPackException);
}

TEST_F(ExceptionTest, SetString) {
    String<2> s;
    std::string str = "Hello";
    EXPECT_THROW(s.set(str), CerealPackException);
}
