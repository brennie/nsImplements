#ifndef ConcreteFooBar_h
#define ConcreteFooBar_h

#include <iostream>

#include "RefCounted.h"
#include "nsIBar.h"
#include "nsIBaz.h"
#include "nsIFoo.h"
#include "nsITornOff.h"
#include "nsImplements.h"

inline namespace {
class TornOffFooBar final
    : public nsImplements<TornOffFooBar, RefCountKind::NonAtomic, nsITornOff> {
public:
  virtual nsresult Quux() override {
    std::cout << "TornOffFooBar::Quux()\n";

    return NS_OK;
  }
};
} // namespace

class ConcreteFooBar final
    : public nsImplements<ConcreteFooBar, RefCountKind::NonAtomic, nsIFoo,
                          nsIBar, implements_traits::TearOff<nsITornOff>,
                          implements_traits::Conditional<nsIBaz>> {
public:
  void SetIsBaz(bool isBaz) { mIsBaz = isBaz; }
  virtual nsresult Frobnicate() override {
    std::cout << "ConcreteFooBar::Frobnicate()\n";

    return NS_OK;
  }

  virtual nsresult Corge() override {
    std::cout << "ConcreteFooBar::Corge()\n";

    return NS_OK;
  }

  virtual nsresult Grault() override {
    std::cout << "ConcreteFoobar::Grault()\n";

    return NS_OK;
  }

  template <typename T> nsISupports *TearOff();

  template <> nsISupports *TearOff<nsITornOff>() { return new TornOffFooBar(); }

  template <typename T> bool ConditionalQueryInterface() const;

  template <> bool ConditionalQueryInterface<nsIBaz>() const { return mIsBaz; }

  virtual ~ConcreteFooBar() {}

private:
  bool mIsBaz = 0;
};

#endif // ConcreteFooBar_h
