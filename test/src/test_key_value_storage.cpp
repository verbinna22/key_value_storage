#include <gtest/gtest.h>

#include <tuple>
#include <vector>

#include "key_value_storage.h"
#include "mock_timer.h"

struct KVSInitSimple : public testing::Test {
    MockTimer *timer;
    KVStorage<MockTimer> *storage;

    void SetUp() {
        timer = new MockTimer();
        auto entries = std::vector{
            std::make_tuple<std::string, std::string>("a", "k1", 1U)};
        storage = new KVStorage(entries, timer);
    }
    void TearDown() {
        delete storage;
        delete timer;
    }
};
