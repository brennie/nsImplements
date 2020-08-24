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

  // nsCOMPtr<nsIBar> bar{ static_cast<nsIBar*>(foo.get()) };
  // bar->Corge();
  return 0;
}
