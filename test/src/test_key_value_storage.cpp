#include <gtest/gtest.h>

#include <optional>
#include <tuple>
#include <vector>

#include "key_value_storage.h"
#include "mock_timer.h"

struct KVSInitSimple : public testing::Test {
    MockTimer timer;
    KVStorage<MockTimerWrapper> *storage;

    void SetUp() {
        auto entries = std::vector{
            std::make_tuple<std::string, std::string>("a", "k1", 1U),
            std::make_tuple<std::string, std::string>("b", "k2", 5U),
            std::make_tuple<std::string, std::string>("c", "k3", 4U),
            std::make_tuple<std::string, std::string>("d", "k4", 6U),
            std::make_tuple<std::string, std::string>("e", "k5", 0U)};
        EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
        storage = new KVStorage(entries, MockTimerWrapper(&timer));
    }
    void TearDown() { delete storage; }
};

struct KVSInitBroad : public testing::Test {
    MockTimer timer;
    KVStorage<MockTimerWrapper> *storage;

    void SetUp() {
        auto entries = std::vector{
            std::make_tuple<std::string, std::string>("a", "k1", 1U),
            std::make_tuple<std::string, std::string>("b", "k2", 5U),
            std::make_tuple<std::string, std::string>("d", "k4", 6U),
            std::make_tuple<std::string, std::string>("e", "k5", 0U)};
        EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
        storage = new KVStorage(entries, MockTimerWrapper(&timer));
    }
    void TearDown() { delete storage; }
};

TEST_F(KVSInitSimple, get_existedValueA_returnsK1) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));

    EXPECT_EQ(storage->get("a"), "k1");
}

TEST_F(KVSInitSimple, get_expiredValueAInTime_returnsNullopt) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(1ULL));

    EXPECT_EQ(storage->get("a"), std::nullopt);
}

TEST_F(KVSInitSimple, get_expiredValueAfterTime_returnsNullopt) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(2ULL));

    EXPECT_EQ(storage->get("a"), std::nullopt);
}

TEST_F(KVSInitSimple, get_existedValueE_returnsK5) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));

    EXPECT_EQ(storage->get("e"), "k5");
}

TEST_F(KVSInitSimple, get_existedValueEAfter200_returnsK5) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(200ULL));

    EXPECT_EQ(storage->get("e"), "k5");
}

TEST_F(KVSInitSimple, set_nonExistedValueW_returnsKNew) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(7ULL))
        .WillOnce(testing::Return(7ULL));

    storage->set("w", "kNew", 6U);

    EXPECT_EQ(storage->get("w"), "kNew");
}

TEST_F(KVSInitSimple, set_nonExistedValueWAfterExpiration_returnsNullopt) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(2ULL))
        .WillOnce(testing::Return(8ULL));

    storage->set("w", "kNew", 6U);

    EXPECT_EQ(storage->get("w"), std::nullopt);
}

TEST_F(KVSInitSimple, set_nonExistedValueWForeverTime_returnsKNew) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(0ULL))
        .WillOnce(testing::Return(0ULL));

    storage->set("w", "kNew", 0U);

    EXPECT_EQ(storage->get("w"), "kNew");
}

TEST_F(KVSInitSimple, set_nonExistedValueWForeverTimeAfterDelay_returnsKNew) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(100ULL))
        .WillOnce(testing::Return(1200ULL));

    storage->set("w", "kNew", 0U);

    EXPECT_EQ(storage->get("w"), "kNew");
}

TEST_F(KVSInitSimple, set_existedValueA_returnsKNew) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(7ULL))
        .WillOnce(testing::Return(7ULL));

    storage->set("a", "kNew", 6U);

    EXPECT_EQ(storage->get("a"), "kNew");
}

TEST_F(KVSInitSimple, set_existedValueAAfterExpiration_returnsNullopt) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(2ULL))
        .WillOnce(testing::Return(8ULL));

    storage->set("a", "kNew", 6U);

    EXPECT_EQ(storage->get("a"), std::nullopt);
}

TEST_F(KVSInitSimple, set_existedValueAForeverTime_returnsKNew) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(0ULL))
        .WillOnce(testing::Return(0ULL));

    storage->set("a", "kNew", 0U);

    EXPECT_EQ(storage->get("a"), "kNew");
}

TEST_F(KVSInitSimple, set_existedValueAForeverTimeAfterDelay_returnsKNew) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(100ULL))
        .WillOnce(testing::Return(1200ULL));

    storage->set("a", "kNew", 0U);

    EXPECT_EQ(storage->get("a"), "kNew");
}

TEST_F(KVSInitSimple, remove_existedValueA_returnsTrue) {
    EXPECT_TRUE(storage->remove("a"));
}

TEST_F(KVSInitSimple, remove_existedValueA_doesNotContainA) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    EXPECT_TRUE(storage->remove("a"));
    EXPECT_EQ(storage->get("a"), std::nullopt);
}

TEST_F(KVSInitSimple, remove_nonExistedValueU_returnsFalse) {
    EXPECT_FALSE(storage->remove("u"));
}

TEST_F(KVSInitSimple, getManySorted_getAllFromStartInTheSameTime_returnsAll) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("a", 5);

    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0].first, "a");
    EXPECT_EQ(values[1].first, "b");
    EXPECT_EQ(values[2].first, "c");
    EXPECT_EQ(values[3].first, "d");
    EXPECT_EQ(values[4].first, "e");
    EXPECT_EQ(values[0].second, "k1");
    EXPECT_EQ(values[1].second, "k2");
    EXPECT_EQ(values[2].second, "k3");
    EXPECT_EQ(values[3].second, "k4");
    EXPECT_EQ(values[4].second, "k5");
}

TEST_F(KVSInitSimple,
       getManySorted_getMoreThanAllFromStartInTheSameTime_returnsAll) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("a", 6);

    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0].first, "a");
    EXPECT_EQ(values[1].first, "b");
    EXPECT_EQ(values[2].first, "c");
    EXPECT_EQ(values[3].first, "d");
    EXPECT_EQ(values[4].first, "e");
    EXPECT_EQ(values[0].second, "k1");
    EXPECT_EQ(values[1].second, "k2");
    EXPECT_EQ(values[2].second, "k3");
    EXPECT_EQ(values[3].second, "k4");
    EXPECT_EQ(values[4].second, "k5");
}

TEST_F(KVSInitSimple, getManySorted_getFourFromStartInTheSameTime_returnsFour) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("a", 4);

    EXPECT_EQ(values.size(), 4);
    EXPECT_EQ(values[0].first, "a");
    EXPECT_EQ(values[1].first, "b");
    EXPECT_EQ(values[2].first, "c");
    EXPECT_EQ(values[3].first, "d");
    EXPECT_EQ(values[0].second, "k1");
    EXPECT_EQ(values[1].second, "k2");
    EXPECT_EQ(values[2].second, "k3");
    EXPECT_EQ(values[3].second, "k4");
}

TEST_F(KVSInitSimple, getManySorted_getOneFromStartInTheSameTime_returnsOne) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("a", 1);

    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values[0].first, "a");
    EXPECT_EQ(values[0].second, "k1");
}

TEST_F(KVSInitSimple, getManySorted_getZeroFromStartInTheSameTime_returnsZero) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("a", 0);

    EXPECT_EQ(values.size(), 0);
}

TEST_F(KVSInitSimple, getManySorted_getAllFromCInTheSameTime_returnsThree) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("c", 5);

    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0].first, "c");
    EXPECT_EQ(values[1].first, "d");
    EXPECT_EQ(values[2].first, "e");
    EXPECT_EQ(values[0].second, "k3");
    EXPECT_EQ(values[1].second, "k4");
    EXPECT_EQ(values[2].second, "k5");
}

TEST_F(KVSInitSimple, getManySorted_getTwoFromCInTheSameTime_returnsTwo) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("c", 2);

    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0].first, "c");
    EXPECT_EQ(values[1].first, "d");
    EXPECT_EQ(values[0].second, "k3");
    EXPECT_EQ(values[1].second, "k4");
}

TEST_F(KVSInitSimple, getManySorted_getZeroFromCInTheSameTime_returnsZero) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("c", 0);

    EXPECT_EQ(values.size(), 0);
}

TEST_F(KVSInitBroad, getManySorted_getTwoFromCInTheSameTime_returnsTwo) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(0ULL));
    auto values = storage->getManySorted("c", 2);

    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values[0].first, "d");
    EXPECT_EQ(values[1].first, "e");
    EXPECT_EQ(values[0].second, "k4");
    EXPECT_EQ(values[1].second, "k5");
}

TEST_F(KVSInitSimple,
       getManySorted_getAllFromStartAfterDelay_returnsUnexpired) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(4ULL));
    auto values = storage->getManySorted("a", 5);

    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0].first, "b");
    EXPECT_EQ(values[1].first, "d");
    EXPECT_EQ(values[2].first, "e");
    EXPECT_EQ(values[0].second, "k2");
    EXPECT_EQ(values[1].second, "k4");
    EXPECT_EQ(values[2].second, "k5");
}

TEST_F(KVSInitSimple,
       getManySorted_getAllFromStartAfterGreatDelay_returnsUnexpired) {
    EXPECT_CALL(timer, getTime()).WillOnce(testing::Return(400ULL));
    auto values = storage->getManySorted("a", 5);

    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values[0].first, "e");
    EXPECT_EQ(values[0].second, "k5");
}
