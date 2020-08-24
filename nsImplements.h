#ifndef nsImplements_h
#define nsImplements_h

#include <type_traits>

#include "RefCounted.h"
#include "nsISupports.h"
#include "nsresult.h"

inline namespace {
  template<typename Derived, typename... Ts>
  struct QueryInterfaceImpl;

  template<typename Derived, typename T, typename...Ts>
  struct QueryInterfaceImpl<Derived, T, Ts...> {
    void* mThat;
    const nsID& mUUID;

    QueryInterfaceImpl(void* that, const nsID& uuid)
        : mThat(that)
        , mUUID(uuid) {}

    nsISupports* operator()() {
      std::cerr << "Comparing T::INTERFACE_ID (" << T::INTERFACE_ID << ") vs desired " << mUUID << std::endl;
      if (T::INTERFACE_ID == mUUID) {
        ptrdiff_t offset = (
          reinterpret_cast<char*>(static_cast<T*>((Derived*)0x1000)) -
          reinterpret_cast<char*>((Derived*)0x1000)
        );

        return reinterpret_cast<nsISupports*>(
          reinterpret_cast<char*>(mThat) + offset
        );
      }

      return QueryInterfaceImpl<Derived, Ts...>(mThat, mUUID)();
    }

  };

  template<typename Derived>
  struct QueryInterfaceImpl<Derived> {
    QueryInterfaceImpl(void*, const nsID&) {}

    nsISupports* operator()() {
      std::cerr << "Does not implement interface\n";
      return nullptr;
    }
  };
}

template<typename Derived, RefCountKind refCountKind, typename... Bases>
class nsImplements : public Bases... {
//  static_assert(
//    std::is_base_of_v<nsISupports, Bases...>,
//    "does not inherit from nsISupports");

 public:
  virtual intptr_t AddRef() override {
    return RefCountTraits<refCountKind>::AddRef(mRefCount);
  }

  virtual intptr_t Release() override {
    intptr_t refCount = RefCountTraits<refCountKind>::Release(mRefCount);
    if (refCount == 0) {
      delete this;
    }
    return refCount;
  }

  virtual nsresult QueryInterface(const nsID& uuid, void** result) override {
    nsISupports* supports = QueryInterfaceImpl<Derived, Bases...>(this, uuid)();
    *result = supports;

    if (supports) {
      supports->AddRef();
      return NS_OK;
    }

    return NS_ERROR_NO_INTERFACE;
  }

  virtual ~nsImplements() = default;

 private:
  typename RefCountTraits<refCountKind>::RefCountT mRefCount;
};


#endif  // nsImplements_h
