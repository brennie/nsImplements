#include <iostream>

#include "ConcreteFooBar.h"
#include "nsCOMPtr.h"
#include "nsIBar.h"
#include "nsIBaz.h"
#include "nsIFoo.h"

int main() {
  nsCOMPtr<ConcreteFooBar> concrete{new ConcreteFooBar()};

  nsCOMPtr<nsIFoo> foo{concrete.get()};
  foo->Frobnicate();

  nsCOMPtr<nsIBar> bar = do_QueryInterface(foo.get());
  bar->Corge();

  nsCOMPtr<nsITornOff> tornOff = do_QueryInterface(foo.get());
  tornOff->Quux();

  nsCOMPtr<nsIBaz> baz = do_QueryInterface(foo.get());
  NS_ASSERT(!baz, "baz should be null");

  concrete->SetIsBaz(true);
  baz = do_QueryInterface(foo.get());
  baz->Grault();

  return 0;
}
