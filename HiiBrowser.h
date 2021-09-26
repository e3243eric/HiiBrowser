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

STATIC
VOID
OUTPUT(
  IN CONST CHAR8  *Format,
  ...
  )
{
  VA_LIST Marker;

  VA_START (Marker, Format);
  DebugVPrint (0xFFFFFFFF, Format, Marker);
  VA_END (Marker);

/*
  VA_START (Marker, Format);
  AsciiPrint (Format, Marker);
  VA_END (Marker);
*/
}

#endif // _HII_BROWSER_H_