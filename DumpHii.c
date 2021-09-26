#include "HiiBrowser.h"
#include "DumpHii.h"

GLOBAL_REMOVE_IF_UNREFERENCED const TYPE_TO_TEXT_TABLE mHiiPackageToTextTable[] = {
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
  {EFI_HII_PACKAGE_TYPE_SYSTEM_END,   L"SystemEnd package"}
};

GLOBAL_REMOVE_IF_UNREFERENCED const TYPE_TO_TEXT_TABLE mHiiSibtToTextTable[] = {
  {EFI_HII_SIBT_END,                  L"End of string information"},
  {EFI_HII_SIBT_STRING_SCSU,          L"Single string information"},
  {EFI_HII_SIBT_STRING_SCSU_FONT,     L"Single string with font information"},
  {EFI_HII_SIBT_STRINGS_SCSU,         L"Multiple strings information"},
  {EFI_HII_SIBT_STRINGS_SCSU_FONT,    L"Multiple strings with font information"},
  {EFI_HII_SIBT_STRING_UCS2,          L"Single UCS-2 string information"},
  {EFI_HII_SIBT_STRING_UCS2_FONT,     L"Single UCS-2 string with font information"},
  {EFI_HII_SIBT_STRINGS_UCS2,         L"Multiple UCS-2 strings information"},
  {EFI_HII_SIBT_STRINGS_UCS2_FONT,    L"Multiple UCS-2 strings with font information"},
  {EFI_HII_SIBT_DUPLICATE,            L"Duplicate string"},
  {EFI_HII_SIBT_SKIP2,                L"Skip string identifiers 2"},
  {EFI_HII_SIBT_SKIP1,                L"Skip string identifiers 1"},
  {EFI_HII_SIBT_EXT1,                 L"Expansion 1"},
  {EFI_HII_SIBT_EXT2,                 L"Expansion 2"},
  {EFI_HII_SIBT_EXT4,                 L"Expansion 4"},
  {EFI_HII_SIBT_FONT,                 L"Font information"}
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

  return L"Unknown package type";
}

CHAR16*
StringInfoTypeToText(
  IN UINT8  Type
  )
{
  UINTN Index;

  for (Index = 0; Index < ARRAY_SIZE(mHiiSibtToTextTable); Index++) {
    if (Type == mHiiSibtToTextTable[Index].Type) {
      return mHiiSibtToTextTable[Index].Text;
    }
  }

  return L"Unknown string information type";
}

VOID
DumpHiiStringBlock(
  IN EFI_HII_STRING_BLOCK   *Buffer,
  IN UINTN                  BufferSize
  )
{
  EFI_HII_STRING_BLOCK                  *BlockHeader = Buffer;
  EFI_STRING_ID                         StringIdCurrent = 1;
  UINTN                                 Index = 0;
  CHAR8                                 *AsciiStringText = NULL;
  CHAR16                                *StringText = NULL;
  UINTN                                 StringSize = 0;
  EFI_HII_SIBT_STRING_SCSU_BLOCK        *StringScsuBlock = NULL;
  EFI_HII_SIBT_STRINGS_SCSU_BLOCK       *StringsScsuBlock = NULL;
  EFI_HII_SIBT_STRING_SCSU_FONT_BLOCK   *StringScsuFontBlock = NULL;
  EFI_HII_SIBT_STRINGS_SCSU_FONT_BLOCK  *StringsScsuFontBlock = NULL;
  EFI_HII_SIBT_STRING_UCS2_BLOCK        *StringUcs2Block = NULL;
  EFI_HII_SIBT_STRINGS_UCS2_BLOCK       *StringsUcs2Block = NULL;
  EFI_HII_SIBT_STRING_UCS2_FONT_BLOCK   *StringUcs2FontBlock = NULL;
  EFI_HII_SIBT_STRINGS_UCS2_FONT_BLOCK  *StringsUcs2FontBlock = NULL;
  EFI_HII_SIBT_DUPLICATE_BLOCK          *DuplicateBlock = NULL;
  EFI_HII_SIBT_SKIP1_BLOCK              *Skip1Block = NULL;
  EFI_HII_SIBT_SKIP2_BLOCK              *Skip2Block = NULL;
  EFI_HII_SIBT_EXT1_BLOCK               *Ext1Block = NULL;
  EFI_HII_SIBT_EXT2_BLOCK               *Ext2Block = NULL;
  EFI_HII_SIBT_EXT4_BLOCK               *Ext4Block = NULL;
  EFI_HII_SIBT_FONT_BLOCK               *FontBlock = NULL;

  while ((UINTN) BlockHeader < (UINTN) Buffer + BufferSize) {
    OUTPUT("      BlockType:0x%X\n", BlockHeader->BlockType);
    switch (BlockHeader->BlockType)
    {
    case EFI_HII_SIBT_END:
      return;

    case EFI_HII_SIBT_STRING_SCSU:
      StringScsuBlock = (EFI_HII_SIBT_STRING_SCSU_BLOCK*) BlockHeader;
      AsciiStringText = (CHAR8*) StringScsuBlock->StringText;
      StringSize = AsciiStrSize(AsciiStringText);

      OUTPUT("      String[%d]:%a\n", StringIdCurrent++, AsciiStringText);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (BlockHeader + 1) + StringSize);
      break;

    case EFI_HII_SIBT_STRINGS_SCSU:
      StringsScsuBlock = (EFI_HII_SIBT_STRINGS_SCSU_BLOCK*) BlockHeader;
      AsciiStringText = (CHAR8*) StringsScsuBlock->StringText;
      StringSize = 0;

      OUTPUT("      StringCount:%d\n", StringsScsuBlock->StringCount);
      for (Index = 0; Index < StringsScsuBlock->StringCount; Index++) {
          OUTPUT("      String[%d]:%a\n", StringIdCurrent++, AsciiStringText);
          StringSize += AsciiStrSize(AsciiStringText);
          AsciiStringText += AsciiStrLen(AsciiStringText);
      }

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (StringsScsuBlock + 1) - sizeof(CHAR8) + StringSize);
      break;

    case EFI_HII_SIBT_STRING_SCSU_FONT:
      StringScsuFontBlock = (EFI_HII_SIBT_STRING_SCSU_FONT_BLOCK*) BlockHeader;
      AsciiStringText = (CHAR8*) StringScsuFontBlock->StringText;
      StringSize = AsciiStrSize(AsciiStringText);

      OUTPUT("      FontIdentifier:%d\n", StringScsuFontBlock->FontIdentifier);
      OUTPUT("      String[%d]:%a\n", StringIdCurrent++, AsciiStringText);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (StringScsuFontBlock + 1) - sizeof(CHAR8) + StringSize);
      break;

    case EFI_HII_SIBT_STRINGS_SCSU_FONT:
      StringsScsuFontBlock = (EFI_HII_SIBT_STRINGS_SCSU_FONT_BLOCK*) BlockHeader;
      AsciiStringText = (CHAR8*) StringsScsuFontBlock->StringText;
      StringSize = 0;

      OUTPUT("      FontIdentifier:%d\n", StringsScsuFontBlock->FontIdentifier);
      OUTPUT("      StringCount:%d\n", StringsScsuFontBlock->StringCount);
      for (Index = 0; Index < StringsScsuFontBlock->StringCount; Index++) {
          OUTPUT("      String[%d]:%a\n", StringIdCurrent++, AsciiStringText);
          StringSize += AsciiStrSize(AsciiStringText);
          AsciiStringText += AsciiStrLen(AsciiStringText);
      }

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (StringsScsuFontBlock + 1) - sizeof(CHAR8) + StringSize);
      break;

    case EFI_HII_SIBT_STRING_UCS2:
      StringUcs2Block = (EFI_HII_SIBT_STRING_UCS2_BLOCK*) BlockHeader;
      StringText = StringUcs2Block->StringText;
      StringSize = StrSize(StringText);

      OUTPUT("      string[%d]:%s\n", StringIdCurrent++, StringText);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (BlockHeader + 1) + StringSize);
      break;

    case EFI_HII_SIBT_STRINGS_UCS2:
      StringsUcs2Block = (EFI_HII_SIBT_STRINGS_UCS2_BLOCK*) BlockHeader;
      StringText = StringsUcs2Block->StringText;
      StringSize = 0;

      OUTPUT("      StringCount:%d\n", StringsUcs2Block->StringCount);
      for (Index = 0; Index < StringsUcs2Block->StringCount; Index++) {
        OUTPUT("      string[%d]:%s\n", StringIdCurrent++, StringText);
        StringSize += StrSize(StringText);
        StringText += StrLen(StringText);
      }

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (StringsUcs2Block + 1) - sizeof(CHAR16) + StringSize);
      break;

    case EFI_HII_SIBT_STRING_UCS2_FONT:
      StringUcs2FontBlock = (EFI_HII_SIBT_STRING_UCS2_FONT_BLOCK*) BlockHeader;
      StringText = StringUcs2FontBlock->StringText;
      StringSize = StrSize(StringText);

      OUTPUT("      FontIdentifier:%d\n", StringUcs2FontBlock->FontIdentifier);
      OUTPUT("      string[%d]\n", StringIdCurrent++, StringText);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (StringUcs2FontBlock + 1) - sizeof(CHAR16) + StringSize);
      break;

    case EFI_HII_SIBT_STRINGS_UCS2_FONT:
      StringsUcs2FontBlock = (EFI_HII_SIBT_STRINGS_UCS2_FONT_BLOCK*) BlockHeader;
      StringText = StringsUcs2FontBlock->StringText;
      StringSize = 0;

      OUTPUT("      FontIdentifier:%d\n", StringsUcs2FontBlock->FontIdentifier);
      OUTPUT("      StringCount:%d\n", StringsUcs2FontBlock->StringCount);
      for (Index = 0; Index < StringsUcs2FontBlock->StringCount; Index++) {
        OUTPUT("      string[%d]:%s\n", StringIdCurrent++, StringText);
        StringSize += StrSize(StringText);
        StringText += StrLen(StringText);
      }

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (StringsUcs2FontBlock + 1) - sizeof(CHAR16) + StringSize);
      break;

    case EFI_HII_SIBT_DUPLICATE:
      DuplicateBlock = (EFI_HII_SIBT_DUPLICATE_BLOCK*) BlockHeader;

      OUTPUT("      StringId:%d\n", DuplicateBlock->StringId);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (DuplicateBlock + 1));
      return;

    case EFI_HII_SIBT_SKIP2:
      Skip2Block = (EFI_HII_SIBT_SKIP2_BLOCK*) BlockHeader;

      OUTPUT("      SkipCount:%d\n", Skip2Block->SkipCount);
      StringIdCurrent += Skip2Block->SkipCount;

      BlockHeader = (EFI_HII_STRING_BLOCK*) (Skip2Block + 1);
      break;

    case EFI_HII_SIBT_SKIP1:
      Skip1Block = (EFI_HII_SIBT_SKIP1_BLOCK*) BlockHeader;

      OUTPUT("      SkipCount:%d\n", Skip1Block->SkipCount);
      StringIdCurrent += Skip1Block->SkipCount;

      BlockHeader = (EFI_HII_STRING_BLOCK*) (Skip1Block + 1);
      break;

    case EFI_HII_SIBT_EXT1:
      Ext1Block = (EFI_HII_SIBT_EXT1_BLOCK*) BlockHeader;

      OUTPUT("      Length:%d\n", Ext1Block->Length);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) BlockHeader + Ext1Block->Length);
      break;

    case EFI_HII_SIBT_EXT2:
      Ext2Block = (EFI_HII_SIBT_EXT2_BLOCK*) BlockHeader;

      OUTPUT("      Length:%d\n", Ext2Block->Length);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) BlockHeader + Ext2Block->Length);
      break;

    case EFI_HII_SIBT_EXT4:
      Ext4Block = (EFI_HII_SIBT_EXT4_BLOCK*) BlockHeader;

      OUTPUT("      Length:%d\n", Ext4Block->Length);
      
      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) BlockHeader + Ext4Block->Length);
      break;

    case EFI_HII_SIBT_FONT:
      FontBlock = (EFI_HII_SIBT_FONT_BLOCK*) BlockHeader;
      StringText = FontBlock->FontName;
      StringSize = StrSize(FontBlock->FontName);

      OUTPUT("      FontId:%d\n", FontBlock->FontId);
      OUTPUT("      FontSize:%d\n", FontBlock->FontSize);
      OUTPUT("      FontStyle:%d\n", FontBlock->FontStyle);
      OUTPUT("      FontStyle:%s\n", StringText);

      BlockHeader = (EFI_HII_STRING_BLOCK*) ((UINTN) (FontBlock + 1) - sizeof(CHAR16) + StringSize);
      break;

    default:
      break;
    }
  }
}

VOID
DumpHiiPackage(
  IN EFI_HII_PACKAGE_HEADER *Buffer,
  IN UINTN                  BufferSize
  )
{
  EFI_HII_PACKAGE_HEADER  *PackageHeader = NULL;
  UINTN                   PackageLength = 0;
  UINTN                   Index = 0;
  
  for (PackageHeader = Buffer;
        (UINTN) PackageHeader < (UINTN) Buffer + BufferSize;
        PackageHeader = (EFI_HII_PACKAGE_HEADER*) ((UINTN) PackageHeader + PackageHeader->Length)) { 

    switch (PackageHeader->Type)
    {
    case EFI_HII_PACKAGE_TYPE_ALL:
      break;

    case EFI_HII_PACKAGE_TYPE_GUID:
      EFI_GUID  *Guid = (EFI_GUID*) (PackageHeader + 1);

      PackageLength = 0;
      while (PackageLength < PackageHeader->Length) {
        OUTPUT("    Guid:%g\n", Guid);
        Guid++;
        PackageLength += sizeof(EFI_GUID);
      }
      break;

    case EFI_HII_PACKAGE_FORMS:
      break;
    
    case EFI_HII_PACKAGE_STRINGS:
      EFI_HII_STRING_PACKAGE_HDR  *StringPackage = (EFI_HII_STRING_PACKAGE_HDR*) PackageHeader;
      EFI_HII_STRING_BLOCK        *StringBlock = (EFI_HII_STRING_BLOCK*) ((UINTN) StringPackage + StringPackage->StringInfoOffset);

      OUTPUT("    HdrSize:0x%X\n", StringPackage->HdrSize);
      OUTPUT("    StringInfoOffset:0x%X\n", StringPackage->StringInfoOffset);
      for (Index = 0; Index < 16; Index++) {
        OUTPUT("    LanguageWindow[%d]:0x%X\n", Index, StringPackage->LanguageWindow[Index]);
      }
      OUTPUT("    LanguageName:0x%X\n", StringPackage->LanguageName);
      OUTPUT("    Language:%a\n", StringPackage->Language);

      DumpHiiStringBlock(StringBlock, (PackageHeader->Length - StringPackage->HdrSize));
      break;

    case EFI_HII_PACKAGE_FONTS:
    case EFI_HII_PACKAGE_IMAGES:
    case EFI_HII_PACKAGE_SIMPLE_FONTS:
    case EFI_HII_PACKAGE_DEVICE_PATH:
    case EFI_HII_PACKAGE_KEYBOARD_LAYOUT:
    case EFI_HII_PACKAGE_ANIMATIONS:
    case EFI_HII_PACKAGE_END:
    case EFI_HII_PACKAGE_TYPE_SYSTEM_BEGIN:
    case EFI_HII_PACKAGE_TYPE_SYSTEM_END:
    default:
      break;
    }
  }
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
      DumpHiiPackage(PackageHeader, PackageHeader->Length);
    }
  }
}