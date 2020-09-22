#ifndef ConcreteFooBar_h
#define ConcreteFooBar_h

#include <iostream>

#include "RefCounted.h"
#include "nsCOMPtr.h"
#include "nsIBar.h"
#include "nsIBaz.h"
#include "nsIFoo.h"
#include "nsITornOff.h"
#include "nsImplements.h"

inline namespace {

class ConcreteFooBar_Base
    : public nsImplements<ConcreteFooBar_Base, RefCountKind::NonAtomic, nsIFoo,
                          nsIBar, implements_traits::TearOff<nsITornOff>,
                          implements_traits::Conditional<nsIBaz>> {};

class TornOffFooBar final
    : public nsImplements<TornOffFooBar, RefCountKind::NonAtomic, nsITornOff> {
public:
  TornOffFooBar(ConcreteFooBar_Base *parent) : mParent{parent} {}

  virtual nsresult Quux() override;

private:
  nsCOMPtr<ConcreteFooBar_Base> mParent;
};
} // namespace

class ConcreteFooBar final : public ConcreteFooBar_Base {
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

  template <> nsISupports *TearOff<nsITornOff>() {
    return new TornOffFooBar(this);
  }

  template <typename T> bool ConditionalQueryInterface() const;

  template <> bool ConditionalQueryInterface<nsIBaz>() const { return mIsBaz; }

  virtual ~ConcreteFooBar() {}

private:
  bool mIsBaz = 0;
};

inline namespace {
nsresult TornOffFooBar::Quux() {
  std::cout << "TornOffFooBar::Quux() {\n";

  ConcreteFooBar *parent = static_cast<ConcreteFooBar *>(mParent.get());

  parent->Frobnicate();
  parent->Corge();
  parent->Grault();

  std::cout << "TornOffFooBar::Quux() }\n";
  return NS_OK;
}
} // namespace

#endif // ConcreteFooBar_h
