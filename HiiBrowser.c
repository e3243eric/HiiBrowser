#include "HiiBrowser.h"

EFI_STATUS
EFIAPI
HiiBrowserEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINTN                         Index;
  UINTN                         BufferSize;
  EFI_STATUS                    Status;
  EFI_HII_HANDLE                *HiiHandles = NULL;
  EFI_HII_PACKAGE_LIST_HEADER   *HiiPackageList = NULL;

  HiiHandles = HiiGetHiiHandles(NULL);
  if (HiiHandles == NULL) {
    Print(L"There are no HII handles in the HII database.\n");
    return EFI_NOT_FOUND;
  }

  for (Index = 0; HiiHandles[Index] != NULL; Index++) {
    BufferSize = 0;
    HiiPackageList = NULL;
    Status = gHiiDatabase->ExportPackageLists(gHiiDatabase, HiiHandles[Index], &BufferSize, HiiPackageList);
    if (Status == EFI_BUFFER_TOO_SMALL) {
      HiiPackageList = AllocatePool(BufferSize);
      ASSERT (HiiPackageList != NULL);

      Status = gHiiDatabase->ExportPackageLists(gHiiDatabase, HiiHandles[Index], &BufferSize, HiiPackageList);
    }
    if (EFI_ERROR (Status)) {
      Print(L"HiiHandle[%d] ExportPackageLists return %r\n", Index, Status);
      continue;
    }
    ASSERT (HiiPackageList != NULL);

    DumpHiiPackageLists (HiiPackageList, BufferSize);
    FreePool (HiiPackageList);
  }

  FreePool (HiiHandles);

  Print(L"ExportPackageLists by NULL HiiHandle.\n");
  {
    BufferSize = 0;
    HiiPackageList = NULL;
    Status = gHiiDatabase->ExportPackageLists(gHiiDatabase, NULL, &BufferSize, HiiPackageList);
    if (Status == EFI_BUFFER_TOO_SMALL) {
      HiiPackageList = AllocatePool(BufferSize);
      ASSERT (HiiPackageList != NULL);

      Status = gHiiDatabase->ExportPackageLists(gHiiDatabase, NULL, &BufferSize, HiiPackageList);
    }
    if (EFI_ERROR (Status)) {
      Print(L"HiiHandle[%d] ExportPackageLists return %r\n", Index, Status);
      return Status;
    }
    DumpHiiPackageLists (HiiPackageList, BufferSize);
    FreePool (HiiPackageList);
  }

  return EFI_SUCCESS;
}
