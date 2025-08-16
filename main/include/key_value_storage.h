#ifndef KEY_VALUE_STORAGE_H
#define KEY_VALUE_STORAGE_H

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

template <class Clock>
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
    std::map<std::uint32_t, std::string_view> _timestampToKey;
    using timestampIteratorType = decltype(_timestampToKey)::iterator;
    std::map<std::string, std::pair<std::string, timestampIteratorType>>
        _keyToValue;
};

template <class Clock>
inline KVStorage<Clock>::KVStorage(
    std::span<std::tuple<std::string, std::string, std::uint32_t>> entries,
    Clock clock) {
    throw std::runtime_error("Not implemented");
}

template <class Clock>
inline KVStorage<Clock>::~KVStorage() {}

template <class Clock>
inline void KVStorage<Clock>::set(std::string key, std::string value,
                                  std::uint32_t timestamp) {
    throw std::runtime_error("Not implemented");
}

template <class Clock>
inline bool KVStorage<Clock>::remove(std::string_view key) {
    throw std::runtime_error("Not implemented");
}

template <class Clock>
inline std::optional<std::string> KVStorage<Clock>::get(
    std::string_view key) const {
    throw std::runtime_error("Not implemented");
}

template <class Clock>
inline std::vector<std::pair<std::string, std::string>>
KVStorage<Clock>::getManySorted(std::string_view key,
                                std::uint32_t count) const {
    throw std::runtime_error("Not implemented");
}

template <class Clock>
inline std::optional<std::pair<std::string, std::string>>
KVStorage<Clock>::removeOneExpiredEntry() {
    throw std::runtime_error("Not implemented");
}

#endif  // KEY_VALUE_STORAGE_H
