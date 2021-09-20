#include "HiiBrowser.h"

VOID
DumpHiiPackageLists (
  IN EFI_HII_PACKAGE_LIST_HEADER  *Buffer,
  IN UINTN                        BufferSize
  )
{
  EFI_HII_PACKAGE_LIST_HEADER   *ListHeader = NULL;
  EFI_HII_PACKAGE_HEADER        *PackageHeader = NULL;
  UINTN                         Count = 0;

  Print(L"PackageLists:0x%X, Size:0x%X\n", Buffer, BufferSize);

  for (ListHeader = Buffer;
        (UINTN) ListHeader < (UINTN) Buffer + BufferSize;
        ListHeader = (EFI_HII_PACKAGE_LIST_HEADER*) ((UINTN) ListHeader + ListHeader->PackageLength)) {
    Print(L"  PackageLists PackageListGuid:%g, PackageLength:0x%X\n", ListHeader->PackageListGuid, ListHeader->PackageLength);

    Count = 0;
    for (PackageHeader = (EFI_HII_PACKAGE_HEADER*) (ListHeader+1);
          (UINTN) PackageHeader < (UINTN) ListHeader + ListHeader->PackageLength;
          PackageHeader = (EFI_HII_PACKAGE_HEADER*) ((UINTN) PackageHeader + PackageHeader->Length)) {
      Print(L"    PackageHeader[%d] Type:0x%X, Length:0x%X\n", Count++, PackageHeader->Type, PackageHeader->Length);
    }
  }
}