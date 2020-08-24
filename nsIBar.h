#ifndef nsIBar_h
#define nsIBar_h

#include "nsresult.h"
#include "nsISupports.h"

class nsIBar : public nsISupports {
 public:
  virtual nsresult Corge() = 0;

  static constexpr nsID INTERFACE_ID = {
    0xCCCCCCCC, 0xCCCC, 0xCCCC, {
      0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC
    }
  };
};

#endif

