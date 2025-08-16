#ifndef MOCK_TIMER_H
#define MOCK_TIMER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>

class MockTimer {
    MOCK_METHOD(std::uint64_t, getTime, ());
};

#endif  // MOCK_TIMER_H
