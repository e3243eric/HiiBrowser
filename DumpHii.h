
#ifndef _UEFI_DEVICE_PATH_LIB_H_
#define _UEFI_DEVICE_PATH_LIB_H_

#include <Uefi.h>

//
// Private Data structure
//
typedef struct {
  UINT8   Type;
  CHAR16  *Text;
} TYPE_TO_TEXT_TABLE;

#endif
