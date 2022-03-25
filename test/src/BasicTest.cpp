#include <gtest/gtest.h>
#include <OneBool.hpp>
#include <cereal_pack.hpp>
#include <cereal_pack_test/test/SimpleTest.hpp>
#include <cereal_pack_test/test/nesting/Nesting.hpp>

class BasicTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(BasicTest, CanConstructGeneratedClasses) {
    cereal_pack_test::test::SimpleTest s;
    OneBool b;
    cereal_pack_test::test::nesting::Nesting n;
}
