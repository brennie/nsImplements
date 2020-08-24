#ifndef nsImplements_h
#define nsImplements_h

#include <type_traits>

#include "RefCounted.h"
#include "nsISupports.h"
#include "nsresult.h"

namespace implements_traits {
  template<typename T>
  struct TearOff {};
}

inline namespace {
  template<typename Derived, typename... Ts>
  struct QueryInterfaceImpl;

  template<typename Derived, typename T, typename... Ts>
  struct QueryInterfaceImpl<Derived, T, Ts...> {
    nsISupports* operator()(void* that, const nsID& uuid) {
      std::cerr << "Comparing T::INTERFACE_ID (" << T::INTERFACE_ID << ") vs desired " << uuid << std::endl;
      if (T::INTERFACE_ID == uuid) {
        ptrdiff_t offset = (
          reinterpret_cast<char*>(static_cast<T*>((Derived*)0x1000)) -
          reinterpret_cast<char*>((Derived*)0x1000)
        );

        return reinterpret_cast<nsISupports*>(
          reinterpret_cast<char*>(that) + offset
        );
      }

      return QueryInterfaceImpl<Derived, Ts...>()(that, uuid);
    }
  };

  template<typename Derived, typename T, typename... Ts>
  struct QueryInterfaceImpl<Derived, implements_traits::TearOff<T>, Ts...> {
    nsISupports* operator()(void *that, const nsID& uuid) {
      std::cerr << "Comparing T::INTERFACE_ID (" << T::INTERFACE_ID << ") vs desired " << uuid << std::endl;
      if (T::INTERFACE_ID == uuid) {
        Derived* derived = reinterpret_cast<Derived*>(that);
        nsISupports* supports = derived->template TearOff<T>();
        if (supports) {
          return supports;
        }
      }

      return QueryInterfaceImpl<Derived, Ts...>()(that, uuid);
    }
  };

  template<typename Derived>
  struct QueryInterfaceImpl<Derived> {

    nsISupports* operator()(void*, const nsID&) {
      std::cerr << "Does not implement interface\n";
      return nullptr;
    }
  };

  template<typename... Bases>
  class nsImplements_Bases;

  template<typename Base, typename... Bases>
  class nsImplements_Bases<implements_traits::TearOff<Base>, Bases...> : public nsImplements_Bases<Bases...> {
    static_assert(
      std::is_base_of_v<nsISupports, Base>,
      "base does not inherit from nsISupports"
    );
  };

  template<typename Base, typename... Bases>
  class nsImplements_Bases<Base, Bases...> : public Base
                                           , public nsImplements_Bases<Bases...> {
    static_assert(
      std::is_base_of_v<nsISupports, Base>,
      "base does not inherit from nsISupports"
    );
  };

  template<>
  class nsImplements_Bases<> {};
}

template<typename Derived, RefCountKind refCountKind, typename... Bases>
class nsImplements : public Bases... {

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
    nsISupports* supports = QueryInterfaceImpl<Derived, Bases...>()(this, uuid);
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
