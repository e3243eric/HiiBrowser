#ifndef _HII_BROWSER_H_
#define _HII_BROWSER_H_

#include <Uefi.h>
#include <Library/HiiLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>

VOID
DumpHiiPackageLists (
  IN EFI_HII_PACKAGE_LIST_HEADER  *Buffer,
  IN UINTN                        BufferSize
  );

#endif // _HII_BROWSER_H_