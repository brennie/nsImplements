#ifndef NSCOMPTR_H
#define NSCOMPTR_H

#include <cstddef>
#include <cstdlib>
#include <type_traits>

#include "nsISupports.h"

#define NS_ASSERT(cond, msg)                                                   \
  do {                                                                         \
    if (!(cond)) {                                                             \
      ::std::cerr << "ASSERTION FAILED: " #cond << ": " << msg << std::endl;   \
      ::std::abort();                                                          \
    }                                                                          \
  } while (false);

template <typename T,
          typename = std::enable_if_t<std::is_base_of_v<nsISupports, T>>>
nsISupports *ToSupports(T *ptr) {
  return static_cast<nsISupports *>(ptr);
}

template <typename T> class [[nodiscard]] do_QueryInterface final {
  static_assert(std::is_base_of_v<nsISupports, T>);

public:
  do_QueryInterface(T * aRaw) : mRaw{ToSupports(aRaw)} {}

  nsresult operator()(const nsID &uuid, void **out) const {
    if (mRaw) {
      return mRaw->QueryInterface(uuid, out);
    }

    return NS_ERROR_NULL_POINTER;
  }

private:
  nsISupports *mRaw;
};

template <typename T> class nsCOMPtr {
public:
  static_assert(std::is_base_of_v<nsISupports, T>);

  explicit nsCOMPtr(std::nullptr_t) : mRaw{nullptr} {}
  explicit nsCOMPtr(T *t) : mRaw{t} {
    if (mRaw) {
      mRaw->AddRef();
    }
  }

  template <typename U> nsCOMPtr(do_QueryInterface<U> qi) : mRaw{nullptr} {
    assign_from_qi(qi);
  }

  template <typename U> nsCOMPtr<T> &operator=(do_QueryInterface<U> qi) {
    assign_from_qi(qi);
    return *this;
  }

  ~nsCOMPtr() {
    if (mRaw) {
      mRaw->Release();
    }
  }

  T *get() const { return reinterpret_cast<T *>(mRaw); }

  operator T *() const && = delete;

  [[nodiscard]] T *operator->() const {
    NS_ASSERT(mRaw != nullptr, "cannot deref null ptr");
    return get();
  }

  [[nodiscard]] T &operator*() const {
    NS_ASSERT(mRaw != nullptr, "cannot deref null ptr");
    return get();
  }

  explicit operator bool() const { return !!mRaw; }

private:
  template <typename U> void assign_from_qi(const do_QueryInterface<U> &qi) {
    static_assert(!std::is_same_v<T, U>);
    static_assert(!std::is_base_of_v<T, U>);

    void *newRaw = nullptr;
    if (NS_FAILED(qi(T::INTERFACE_ID, &newRaw))) {
      newRaw = nullptr;
    }

    if (mRaw) {
      mRaw->Release();
    }

    // QueryInterface() takes care of AddRef.
    mRaw = static_cast<T *>(newRaw);
  }

private:
  T *mRaw;
};

#endif // NSCOMPTR_H
