#ifndef MOCK_TIMER_H
#define MOCK_TIMER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>

class MockTimer {
public:
    MOCK_METHOD(std::uint64_t, getTime, (), (const));
};

class MockTimerWrapper {
public:
    explicit MockTimerWrapper(MockTimer *timer): _timer(timer) {}

    std::uint64_t getTime() const {
        return _timer->getTime();
    }
private:
    MockTimer *_timer;
};

#endif  // MOCK_TIMER_H
