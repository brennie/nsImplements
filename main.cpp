#include <iostream>

#include "ConcreteFooBar.h"
#include "nsCOMPtr.h"
#include "nsIBar.h"
#include "nsIBaz.h"
#include "nsIFoo.h"

int main() {
  nsCOMPtr<ConcreteFooBar> concrete { new ConcreteFooBar() };

  nsCOMPtr<nsIFoo> foo{ concrete.get() };
  foo->Frobnicate();

  void* qi = nullptr;
  nsresult rv = foo->QueryInterface(nsIBar::INTERFACE_ID, &qi);

  nsCOMPtr<nsIBar> bar{reinterpret_cast<nsIBar *>(qi)};
  bar->Corge();

  qi = nullptr;
  rv = foo->QueryInterface(nsITornOff::INTERFACE_ID, &qi);
  nsCOMPtr<nsITornOff> tornOff{reinterpret_cast<nsITornOff *>(qi)};
  tornOff->Quux();

  qi = nullptr;
  rv = foo->QueryInterface(nsIBaz::INTERFACE_ID, &qi);
  NS_ASSERT(qi == nullptr, "qi should be null");

  concrete->SetIsBaz(true);
  qi = nullptr;
  rv = foo->QueryInterface(nsIBaz::INTERFACE_ID, &qi);
  nsCOMPtr<nsIBaz> baz{reinterpret_cast<nsIBaz *>(qi)};
  baz->Grault();

  return 0;
}
