#ifndef NSCOMPTR_H
#define NSCOMPTR_H

#include <cstddef>
#include <cstdlib>

#define NS_ASSERT(cond, msg)           \
  do {                                 \
    if (!(cond)) {                     \
      ::std::cerr << msg << std::endl; \
      ::std::abort();                  \
    }                                  \
  } while (false);                     \


template<typename T>
class nsCOMPtr {
 public:
  explicit nsCOMPtr(std::nullptr_t) : mRaw{nullptr} {}
  explicit nsCOMPtr(T* t) : mRaw{t} {
    if (mRaw) {
      mRaw->AddRef();
    }
  }

  ~nsCOMPtr() {
    if (mRaw) {
      mRaw->Release();
    }
  }

  T* get() const { return reinterpret_cast<T*>(mRaw); }

  operator T*() const&& = delete;

  T* operator->() const {
    NS_ASSERT(mRaw != nullptr, "cannot deref null ptr");
    return get();
  }

  T& operator*() const {
    NS_ASSERT(mRaw != nullptr, "cannot deref null ptr");
    return get();
  }

  explicit operator bool() const { return !!mRaw; }

 private:
  T* mRaw;
};

#endif  // NSCOMPTR_H
