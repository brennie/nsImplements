#include <iostream>

#include "ConcreteFooBar.h"
#include "nsCOMPtr.h"
#include "nsIFoo.h"

int main() {
  nsCOMPtr<nsIFoo> foo{ new ConcreteFooBar() };
  foo->Frobnicate();

  void* qi = nullptr;
  nsresult rv = foo->QueryInterface(nsIBar::INTERFACE_ID, &qi);

  nsCOMPtr<nsIBar> bar{reinterpret_cast<nsIBar *>(qi)};
  bar->Corge();

  qi = nullptr;
  rv = foo->QueryInterface(nsITornOff::INTERFACE_ID, &qi);
  nsCOMPtr<nsITornOff> tornOff{reinterpret_cast<nsITornOff *>(qi)};
  tornOff->Quux();

  return 0;
}
