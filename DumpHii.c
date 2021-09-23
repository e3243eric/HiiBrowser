#include "HiiBrowser.h"
#include "DumpHii.h"

GLOBAL_REMOVE_IF_UNREFERENCED const HII_PACKAGE_TO_TEXT_TABLE mHiiPackageToTextTable[] = {
  {EFI_HII_PACKAGE_TYPE_ALL,          L"All Pseudo-package"},
  {EFI_HII_PACKAGE_TYPE_GUID,         L"Guid package"},
  {EFI_HII_PACKAGE_FORMS,             L"Forms package"},
  {EFI_HII_PACKAGE_STRINGS,           L"Strings package"},
  {EFI_HII_PACKAGE_FONTS,             L"Fonts package"},
  {EFI_HII_PACKAGE_IMAGES,            L"Image package"},
  {EFI_HII_PACKAGE_SIMPLE_FONTS,      L"SimpleFonts package"},
  {EFI_HII_PACKAGE_DEVICE_PATH,       L"DevicePath package"},
  {EFI_HII_PACKAGE_KEYBOARD_LAYOUT,   L"KeyboardLayout package"},
  {EFI_HII_PACKAGE_ANIMATIONS,        L"Animations package"},
  {EFI_HII_PACKAGE_END,               L"End of package"},
  {EFI_HII_PACKAGE_TYPE_SYSTEM_BEGIN, L"SystemBegin package"},
  {EFI_HII_PACKAGE_TYPE_SYSTEM_END,   L"SystemEnd package"},
  {0, NULL}
};

CHAR16*
PackageTypeToText(
  IN UINT8  Type
  )
{
  UINTN Index;

  for (Index = 0; Index < ARRAY_SIZE(mHiiPackageToTextTable); Index++) {
    if (Type == mHiiPackageToTextTable[Index].Type) {
      return mHiiPackageToTextTable[Index].Text;
    }
  }

  return L"Unknown package";
}

VOID
DumpHiiPackageLists (
  IN EFI_HII_PACKAGE_LIST_HEADER  *Buffer,
  IN UINTN                        BufferSize
  )
{
  EFI_HII_PACKAGE_LIST_HEADER   *ListHeader = NULL;
  EFI_HII_PACKAGE_HEADER        *PackageHeader = NULL;
  UINTN                         PackageCount = 0;

  for (ListHeader = Buffer;
        (UINTN) ListHeader < (UINTN) Buffer + BufferSize;
        ListHeader = (EFI_HII_PACKAGE_LIST_HEADER*) ((UINTN) ListHeader + ListHeader->PackageLength)) {
    OUTPUT("PackageListGuid:%g, PackageLength:0x%X\n", ListHeader->PackageListGuid, ListHeader->PackageLength);

    PackageCount = 0;
    for (PackageHeader = (EFI_HII_PACKAGE_HEADER*) (ListHeader+1);
          (UINTN) PackageHeader < (UINTN) ListHeader + ListHeader->PackageLength;
          PackageHeader = (EFI_HII_PACKAGE_HEADER*) ((UINTN) PackageHeader + PackageHeader->Length)) {
      OUTPUT("  PackageHeader[%d] Type:0x%X (%s), Length:0x%X\n", PackageCount++, PackageHeader->Type, PackageTypeToText((UINT8)PackageHeader->Type), PackageHeader->Length);
    }
  }
}