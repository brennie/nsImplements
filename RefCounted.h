#ifndef RefCounted_h
#define RefCounted_h

#include <atomic>

enum class RefCountKind {
  NonAtomic,
  Atomic,
};

template <RefCountKind> class RefCountTraits;

template <> class RefCountTraits<RefCountKind::NonAtomic> final {
public:
  using RefCountT = intptr_t;
  static inline intptr_t AddRef(RefCountT &refCount) { return ++refCount; }

  static inline intptr_t Release(RefCountT &refCount) { return --refCount; }
};

template <> class RefCountTraits<RefCountKind::Atomic> final {
public:
  using RefCountT = std::atomic<intptr_t>;

  static inline intptr_t AddRef(RefCountT &refCount) {
    intptr_t oldRefCount = refCount.fetch_add(1, std::memory_order_seq_cst);
    return oldRefCount + 1;
  }

  static inline intptr_t Release(RefCountT &refCount) {
    intptr_t oldRefCount = refCount.fetch_sub(1, std::memory_order_seq_cst);
    return oldRefCount - 1;
  }

private:
  RefCountT mRefCount;
};

#endif // RefCounted_h
