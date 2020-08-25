
#ifndef nsIBaz_h
#define nsIBaz_h

#include "nsresult.h"
#include "nsISupports.h"

class nsIBaz : public nsISupports {
 public:
  virtual nsresult Grault() = 0;

  static constexpr nsID INTERFACE_ID = {
    0xBBBBBBBB, 0xBBBB, 0xBBBB, {
      0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB
    }
  };
};

#endif

