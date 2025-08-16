#ifndef KEY_VALUE_STORAGE_H
#define KEY_VALUE_STORAGE_H

#include <concepts>
#include <cstdint>
#include <limits>
#include <map>
#include <optional>
#include <span>  // NOLINT
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

template <class T>
concept TimeGetter = requires(T getter) {
    { getter.getTime() } -> std::convertible_to<std::uint64_t>;
};  // NOLINT

template <TimeGetter Clock>
class KVStorage {
   public:
    explicit KVStorage(
        std::span<std::tuple<std::string, std::string, std::uint32_t>> entries,
        Clock clock = Clock());
    ~KVStorage();

    void set(const std::string &key, const std::string &value,
             std::uint32_t timestamp);
    bool remove(std::string_view key);
    std::optional<std::string> get(std::string_view key) const;
    std::vector<std::pair<std::string, std::string>> getManySorted(
        std::string_view key, std::uint32_t count) const;
    std::optional<std::pair<std::string, std::string>> removeOneExpiredEntry();

   private:
    std::multimap<std::uint32_t, std::string> _timestampToKey;
    using timestampIteratorType = decltype(_timestampToKey)::iterator;
    std::map<std::string_view, std::pair<std::string, timestampIteratorType>>
        _keyToValue;
    Clock _clock;

    void addWithTimestamp(const std::string &key,
                          const std::string &value,
                          std::uint32_t timestamp,
                          std::uint64_t time);
};

template <TimeGetter Clock>
inline KVStorage<Clock>::KVStorage(
    std::span<std::tuple<std::string, std::string, std::uint32_t>> entries,
    Clock clock)
    : _clock(clock) {
    std::uint64_t time = _clock.getTime();
    for (const auto &entry : entries) {
        addWithTimestamp(std::get<0>(entry), std::get<1>(entry),
                         std::get<2>(entry), time);
    }
}

template <TimeGetter Clock>
inline KVStorage<Clock>::~KVStorage() {}

template <TimeGetter Clock>
inline void KVStorage<Clock>::set(const std::string &key,
                                  const std::string &value,
                                  std::uint32_t timestamp) {
    addWithTimestamp(key, value, timestamp, _clock.getTime());
}

template <TimeGetter Clock>
inline bool KVStorage<Clock>::remove(std::string_view key) {
    if (_keyToValue.contains(key)) {
        auto iteratorToKeyToValueIteratorPair = _keyToValue.find(key);
        _timestampToKey.erase(iteratorToKeyToValueIteratorPair->second.second);
        _keyToValue.erase(iteratorToKeyToValueIteratorPair);
        return true;
    }
    return false;
}

template <TimeGetter Clock>
inline std::optional<std::string> KVStorage<Clock>::get(
    std::string_view key) const {
    std::uint64_t time = _clock.getTime();
    if (_keyToValue.contains(key)) {
        auto valueIteratorPair = _keyToValue.at(key);
        if (valueIteratorPair.second->first > time) {
            return valueIteratorPair.first;
        }
    }
    return std::nullopt;
}

template <TimeGetter Clock>
inline std::vector<std::pair<std::string, std::string>>
KVStorage<Clock>::getManySorted(std::string_view key,
                                std::uint32_t count) const {
    std::uint64_t time = _clock.getTime();
    auto iterator = _keyToValue.lower_bound(key);
    std::vector<std::pair<std::string, std::string>> result;
    while (iterator != _keyToValue.end() && count > 0) {
        if (iterator->second.second->first > time) {
            result.emplace_back(iterator->first, iterator->second.first);
            --count;
        }
        ++iterator;
    }
    return result;
}

template <TimeGetter Clock>
inline std::optional<std::pair<std::string, std::string>>
KVStorage<Clock>::removeOneExpiredEntry() {
    std::uint64_t time = _clock.getTime();
    auto iterator = _timestampToKey.begin();
    return std::nullopt;
}

template <TimeGetter Clock>
inline void KVStorage<Clock>::addWithTimestamp(const std::string &key,
                                               const std::string &value,
                                               std::uint32_t timestamp,
                                               std::uint64_t time) {
    auto expirationTime = (timestamp == 0)
                              ? std::numeric_limits<std::uint64_t>::max()
                              : time + timestamp;
    auto iterator = _timestampToKey.insert(std::pair(expirationTime, key));
    _keyToValue[key] = std::make_pair(value, iterator);
}

#endif  // KEY_VALUE_STORAGE_H
