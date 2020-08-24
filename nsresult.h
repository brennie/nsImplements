#ifndef NSRESULT_H
#define NSRESULT_H

enum class nsresult : uint32_t {
  NS_OK = 0,
  NS_ERROR_NO_INTERFACE = 0x80004002,
  NS_ERROR_NULL_POINTER = 0x80070057,
};

#define NS_OK nsresult::NS_OK
#define NS_ERROR_NO_INTERFACE nsresult::NS_ERROR_NO_INTERFACE
#define NS_ERROR_NULL_POINTER nsresult::NS_ERROR_NULL_POINTER

#endif  // NSRESULT_H
