#include <gtest/gtest.h>

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
            std::make_tuple<std::string, std::string>("e", "k5", 0U)
        };
        storage = new KVStorage(entries, MockTimerWrapper(&timer));
    }
    void TearDown() {
        delete storage;
    }
};

TEST_F(KVSInitSimple, get_existedValueA_returnsK1) {
    EXPECT_CALL(timer, getTime())
        .WillOnce(testing::Return(0ULL));

    EXPECT_EQ(storage->get("a"), "k1");
}

// TEST_F(KVSInitSimple, get_expiredValueA_returnsK1) {
//     EXPECT_CALL((*timer), getTime())
//         .WillOnce(testing::Return(0ULL))
//         .WillOnce(testing::Return(0ULL));

//     EXPECT_EQ(storage->get("a"), "k1");
// }
