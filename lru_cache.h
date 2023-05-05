#pragma once

#include <list>
#include <unordered_map>

template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class LRUCache {
public:
  explicit LRUCache(int cap) : cap_(cap) {}
  LRUCache(const LRUCache &) = delete;
  LRUCache(const LRUCache &&) = delete;
  LRUCache &operator=(const LRUCache &) = delete;
  LRUCache &operator=(const LRUCache &&) = delete;
  bool Get(const Key &key, Value *val_ptr);
  void Put(const Key &key, const Value &val);

private:
  void MoveToFront(const Key &key);

  using CacheList = std::list<std::pair<Key, Value>>;
  int cap_;
  CacheList list_;
  std::unordered_map<Key, typename CacheList::iterator, Hash, KeyEqual> map_;
};

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void LRUCache<Key, Value, Hash, KeyEqual>::Put(const Key &key,
                                               const Value &val) {
  if (map_.count(key) == 0) {
    list_.emplace_front(key, val);
    map_[key] = list_.begin();
    if (list_.size() > cap_) {
      map_.erase(list_.back().first);
      list_.pop_back();
    }
  } else {
    MoveToFront(key);
    list_.front().second = val;
  }
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
bool LRUCache<Key, Value, Hash, KeyEqual>::Get(const Key &key, Value *val_ptr) {
  if (map_.count(key) == 0) {
    return false;
  } else {
    MoveToFront(key);
    *val_ptr = list_.front().second;
    return true;
  }
}

template <typename Key, typename Value, typename Hash, typename KeyEqual>
void LRUCache<Key, Value, Hash, KeyEqual>::MoveToFront(const Key &key) {
  auto iter = map_[key];
  if (iter != list_.begin()) {
    list_.splice(list_.cbegin(), list_, iter, std::next(iter));
  }
}
