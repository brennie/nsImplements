#ifndef nsITornOff_h
#define nsITornOff_h

#include "nsresult.h"
#include "nsISupports.h"

class nsITornOff : public nsISupports {
 public:
  virtual nsresult Quux() = 0;

  static constexpr nsID INTERFACE_ID = {
    0xDDDDDDDD, 0xDDDD, 0xDDDD, {
      0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD
    }
  };
};

#endif

