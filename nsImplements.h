#ifndef nsImplements_h
#define nsImplements_h

#include <type_traits>

#include "RefCounted.h"
#include "nsISupports.h"
#include "nsresult.h"

namespace implements_traits {
  template<typename T>
  struct TearOff {};

  template<typename T>
  struct Conditional {};
}

inline namespace {
  template<typename Derived, typename Interface>
  constexpr ptrdiff_t InterfacePointerOffset() {
    return (
      reinterpret_cast<char*>(static_cast<Interface*>((Derived*)0x1000)) -
      reinterpret_cast<char*>((Derived*)0x1000)
    );
  };

  template<typename Derived, typename Interface>
  constexpr nsISupports* InterfaceCast(void* that) {
    return reinterpret_cast<nsISupports*>(
      reinterpret_cast<char*>(that) + InterfacePointerOffset<Derived, Interface>()
    );
  }

  template<typename Derived, typename... Ts>
  struct QueryInterfaceImpl;

  template<typename Derived, typename T, typename... Ts>
  struct QueryInterfaceImpl<Derived, T, Ts...> {
    nsISupports* operator()(void* that, const nsID& uuid) {
#ifdef DEBUG
      std::cerr << "Comparing T::INTERFACE_ID (" << T::INTERFACE_ID << ") vs desired " << uuid << std::endl;
#endif

      if (T::INTERFACE_ID == uuid) {
        return InterfaceCast<Derived, T>(that);
      }

      return QueryInterfaceImpl<Derived, Ts...>()(that, uuid);
    }
  };

  template<typename Derived, typename T, typename... Ts>
  struct QueryInterfaceImpl<Derived, implements_traits::TearOff<T>, Ts...> {
    nsISupports* operator()(void *that, const nsID& uuid) {
#ifdef DEBUG
      std::cerr << "Comparing T::INTERFACE_ID (" << T::INTERFACE_ID << ") vs desired " << uuid << std::endl;
#endif

      if (T::INTERFACE_ID == uuid) {
        Derived* derived = reinterpret_cast<Derived*>(that);
        if (nsISupports* supports = derived->template TearOff<T>()) {
          return supports;
        }
      }

      return QueryInterfaceImpl<Derived, Ts...>()(that, uuid);
    }
  };

  template<typename Derived, typename T, typename... Ts>
  struct QueryInterfaceImpl<Derived, implements_traits::Conditional<T>, Ts...> {
    nsISupports* operator()(void *that, const nsID& uuid) {
#ifdef DEBUG
      std::cerr << "Comparing T::INTERFACE_ID (" << T::INTERFACE_ID << ") vs desired " << uuid << std::endl;
#endif

      if (T::INTERFACE_ID == uuid) {
         if (Derived* derived = reinterpret_cast<Derived*>(that);
             derived->template ConditionalQueryInterface<T>()) {
          return InterfaceCast<Derived, T>(that);
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

  template<>
  class nsImplements_Bases<> {};

  template<typename Base, typename... Bases>
  class nsImplements_Bases<implements_traits::TearOff<Base>, Bases...>
      : public nsImplements_Bases<Bases...> {
    static_assert(
      std::is_base_of_v<nsISupports, Base>,
      "base does not inherit from nsISupports"
    );
  };

  static_assert(!std::is_base_of_v<nsISupports, nsImplements_Bases<implements_traits::TearOff<nsISupports>>>);

  template<typename Base, typename... Bases>
  class nsImplements_Bases<implements_traits::Conditional<Base>, Bases...>
      : public Base
      , public nsImplements_Bases<Bases...> {
    static_assert(
      std::is_base_of_v<nsISupports, Base>,
      "base does not inherit from nsISupports"
    );
  };

  static_assert(std::is_base_of_v<nsISupports, nsImplements_Bases<implements_traits::Conditional<nsISupports>>>);

  template<typename Base, typename... Bases>
  class nsImplements_Bases<Base, Bases...> : public Base
                                           , public nsImplements_Bases<Bases...> {
    static_assert(
      std::is_base_of_v<nsISupports, Base>,
      "base does not inherit from nsISupports"
    );
  };

}

template<typename Derived, RefCountKind refCountKind, typename... Bases>
class nsImplements : public nsImplements_Bases<Bases...> {
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


// inline namespace tests {

// using Bases = nsImplements_Bases<implements_traits::Conditional<nsISupports>>;
// class Temp : public Bases {};
// static_assert(std::is_base_of_v<nsISupports, Bases>);
// static_assert(std::is_base_of_v<nsISupports, Temp>);

// class Temp2 : public nsImplements<Temp2, RefCountKind::NonAtomic, implements_traits::Conditional<nsISupports>>;
// using Implements = nsImplements<Temp2, RefCountKind::NonAtomic, implements_traits::Conditional<nsISupports>>;

// static_assert(
//     std::is_base_of_v<
//       Bases,
//       Implements>,
//     "Implements : public Bases"
// );
// static_assert(
//     std::is_base_of_v<
//       nsISupports,
//       Implements>
//     "nsImplements<..., ..., implements_traits::Conditional<T>> should inherit from T"
// );
// static_assert(
//     std::is_base_of_v<nsISupports, Temp2>,
//     "T : nsImpelements<T, ..., implemenets_traits::Conditional<U>> should inherit from U"
// );

// }

#endif  // nsImplements_h
