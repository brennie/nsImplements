#ifndef nsIFoo_h
#define nsIFoo_h

#include "nsISupports.h"
#include "nsresult.h"

class nsIFoo : public nsISupports {
public:
  virtual nsresult Frobnicate() = 0;

  static constexpr nsID INTERFACE_ID = {
      0xFFFFFFFF,
      0xFFFF,
      0xFFFF,
      {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
};

#endif
