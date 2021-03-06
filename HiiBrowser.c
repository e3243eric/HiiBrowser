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
    OUTPUT("There are no HII handles in the HII database.\n");
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
      OUTPUT("HiiHandle[%d] ExportPackageLists return %r\n", Index, Status);
      continue;
    }
    ASSERT (HiiPackageList != NULL);

    DumpHiiPackageLists (HiiPackageList, BufferSize);
    FreePool (HiiPackageList);
  }

  FreePool (HiiHandles);
  return EFI_SUCCESS;
}
