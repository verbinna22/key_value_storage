#ifndef KEY_VALUE_STORAGE_H
#define KEY_VALUE_STORAGE_H

#include <concepts>
#include <cstdint>
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

    void set(std::string key, std::string value, std::uint32_t timestamp);
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
};

template <TimeGetter Clock>
inline KVStorage<Clock>::KVStorage(
    std::span<std::tuple<std::string, std::string, std::uint32_t>> entries,
    Clock clock)
    : _clock(clock) {
    std::uint64_t time = _clock.getTime();
    for (const auto &entry : entries) {
        auto iterator = _timestampToKey.insert(
            std::pair(time + std::get<2>(entry), std::get<0>(entry)));
        _keyToValue[std::get<0>(entry)] =
            std::make_pair(std::get<1>(entry), iterator);
    }
}

template <TimeGetter Clock>
inline KVStorage<Clock>::~KVStorage() {
}

template <TimeGetter Clock>
inline void KVStorage<Clock>::set(std::string key, std::string value,
                                  std::uint32_t timestamp) {
    throw std::runtime_error("Not implemented");
}

template <TimeGetter Clock>
inline bool KVStorage<Clock>::remove(std::string_view key) {
    throw std::runtime_error("Not implemented");
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
    throw std::runtime_error("Not implemented");
}

template <TimeGetter Clock>
inline std::optional<std::pair<std::string, std::string>>
KVStorage<Clock>::removeOneExpiredEntry() {
    throw std::runtime_error("Not implemented");
}

#endif  // KEY_VALUE_STORAGE_H
