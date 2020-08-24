#ifndef ConcreteFooBar_h
#define ConcreteFooBar_h

#include <iostream>

#include "nsIBar.h"
#include "nsIFoo.h"
#include "RefCounted.h"
#include "nsImplements.h"

class ConcreteFooBar final :
    public nsImplements<ConcreteFooBar, RefCountKind::NonAtomic, nsIFoo, nsIBar>
{
 public:
  virtual nsresult Frobnicate() override {
    std::cout << "ConcreteFooBar::Frobnicate()\n";

    return NS_OK;
  }

  virtual nsresult Corge() override {
    std::cout << "ConcreteFooBar::Corge()\n";

    return NS_OK;
  }

  virtual ~ConcreteFooBar() {}
};

#endif  // ConcreteFooBar_h
