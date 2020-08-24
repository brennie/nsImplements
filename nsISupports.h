#ifndef NSISUPPORTS_H
#define NSISUPPORTS_H

#include <cstdint>

#include "nsID.h"
#include "nsresult.h"

class nsISupports {
 public:
  virtual intptr_t AddRef() = 0;
  virtual intptr_t Release() = 0;
  virtual nsresult QueryInterface(const nsID& uuid, void** result) = 0;

  static constexpr nsID INTERFACE_ID = {
    0x00000000, 0x0000, 0x0000, {
      0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46,
    }
  };
};

#endif  // NSISUPPORTS_H
