#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

struct symbol_entry {
    void *address;
    const char * const name;
};

#define REGISTER_SYMBOL(name) symbol_entry name = { (void *)(0xDEAD0001 + __LINE__ * 2), #name};

// debug
REGISTER_SYMBOL(DEBUG_FONTBASE)
REGISTER_SYMBOL(Debug_GetCursorPosition)
REGISTER_SYMBOL(Debug_Printf)
REGISTER_SYMBOL(Debug_PrintNumberHex_Byte)
REGISTER_SYMBOL(Debug_PrintNumberHex_Dword)
REGISTER_SYMBOL(Debug_PrintNumberHex_Nibble)
REGISTER_SYMBOL(Debug_PrintNumberHex_Word)
REGISTER_SYMBOL(Debug_PrintString)
REGISTER_SYMBOL(Debug_SetCursorPosition)
REGISTER_SYMBOL(Debug_WaitKey)

// file
REGISTER_SYMBOL(File_Close)
REGISTER_SYMBOL(File_FindClose)
REGISTER_SYMBOL(File_FindNext)
REGISTER_SYMBOL(File_FindFirst)
REGISTER_SYMBOL(File_Fstat)
REGISTER_SYMBOL(File_GetAddr)
REGISTER_SYMBOL(File_Lseek)
REGISTER_SYMBOL(File_Mkdir)
REGISTER_SYMBOL(File_Open)
REGISTER_SYMBOL(File_Read)
REGISTER_SYMBOL(File_Remove)
REGISTER_SYMBOL(File_Rename)
REGISTER_SYMBOL(File_Stat)
REGISTER_SYMBOL(File_Write)
REGISTER_SYMBOL(File_Flush)

// gui
REGISTER_SYMBOL(GUIButton_ctor)
REGISTER_SYMBOL(GUIDropDownMenu_ctor)
REGISTER_SYMBOL(GUIDropDownMenuItem_ctor)
REGISTER_SYMBOL(GUIDialog_ctor)
REGISTER_SYMBOL(GUILabel_ctor)
REGISTER_SYMBOL(GUILongLabel_ctor)
REGISTER_SYMBOL(GUIRadioButton_ctor)
REGISTER_SYMBOL(GUITextBox_ctor)
REGISTER_SYMBOL(GUI_DisplayMessageBox)
REGISTER_SYMBOL(GUI_DisplayMessageBox_Internal)

// input
REGISTER_SYMBOL(Input_GetKeyState)
REGISTER_SYMBOL(Input_IsAnyKeyDown)
REGISTER_SYMBOL(GetInput)

// lcd
REGISTER_SYMBOL(LCD_ClearScreen)
REGISTER_SYMBOL(LCD_GetPixel)
REGISTER_SYMBOL(LCD_GetSize)
REGISTER_SYMBOL(LCD_GetVRAMAddress)
REGISTER_SYMBOL(LCD_Refresh)
REGISTER_SYMBOL(LCD_SetPixel)
REGISTER_SYMBOL(LCD_SetPixelFromPalette)
REGISTER_SYMBOL(LCD_VRAMBackup)
REGISTER_SYMBOL(LCD_VRAMRestore)
REGISTER_SYMBOL(LCD_SendCommand)
REGISTER_SYMBOL(LCD_SetDrawingBounds)

// mcs
REGISTER_SYMBOL(MCS_CreateFolder)
REGISTER_SYMBOL(MCS_GetVariable)
REGISTER_SYMBOL(MCS_List_Create)
REGISTER_SYMBOL(MCS_List_Set)
REGISTER_SYMBOL(MCS_SetVariable)

// mem
REGISTER_SYMBOL(Mem_Free)
REGISTER_SYMBOL(Mem_Malloc)
REGISTER_SYMBOL(Mem_Memcpy)
REGISTER_SYMBOL(Mem_Memset)

// serial
REGISTER_SYMBOL(Serial_Open)
REGISTER_SYMBOL(Serial_Close)
REGISTER_SYMBOL(Serial_ReadSingle)
REGISTER_SYMBOL(Serial_Read)
REGISTER_SYMBOL(Serial_Peek)
REGISTER_SYMBOL(Serial_WriteSingle)
REGISTER_SYMBOL(Serial_WriteUnbuffered)
REGISTER_SYMBOL(Serial_Write)
REGISTER_SYMBOL(Serial_PollRX)
REGISTER_SYMBOL(Serial_PollTX)
REGISTER_SYMBOL(Serial_ClearRX)
REGISTER_SYMBOL(Serial_ClearTX)
REGISTER_SYMBOL(Serial_IsOpen)

// string
REGISTER_SYMBOL(String_Strcat)
REGISTER_SYMBOL(String_Strchr)
REGISTER_SYMBOL(String_Strcmp)
REGISTER_SYMBOL(String_Strcpy)
REGISTER_SYMBOL(String_Strlen)


static symbol_entry * const symbols[] {
    // debug
    &DEBUG_FONTBASE,
    &Debug_GetCursorPosition,
    &Debug_Printf,
    &Debug_PrintNumberHex_Byte,
    &Debug_PrintNumberHex_Dword,
    &Debug_PrintNumberHex_Nibble,
    &Debug_PrintNumberHex_Word,
    &Debug_PrintString,
    &Debug_SetCursorPosition,
    &Debug_WaitKey,

    // file
    &File_Close,
    &File_FindClose,
    &File_FindNext,
    &File_FindFirst,
    &File_Fstat,
    &File_GetAddr,
    &File_Lseek,
    &File_Mkdir,
    &File_Open,
    &File_Read,
    &File_Remove,
    &File_Rename,
    &File_Stat,
    &File_Write,

    // gui
    &GUIButton_ctor,
    &GUIDropDownMenu_ctor,
    &GUIDropDownMenuItem_ctor,
    &GUIDialog_ctor,
    &GUILabel_ctor,
    &GUILongLabel_ctor,
    &GUIRadioButton_ctor,
    &GUITextBox_ctor,
    &GUI_DisplayMessageBox,
    &GUI_DisplayMessageBox_Internal,

    // input
    &Input_GetKeyState,
    &Input_IsAnyKeyDown,
    &GetInput,

    // lcd
    &LCD_ClearScreen,
    &LCD_GetPixel,
    &LCD_GetSize,
    &LCD_GetVRAMAddress,
    &LCD_Refresh,
    &LCD_SetPixel,
    &LCD_SetPixelFromPalette,
    &LCD_VRAMBackup,
    &LCD_VRAMRestore,
    &LCD_SendCommand,
    &LCD_SetDrawingBounds,

    // mcs
    &MCS_CreateFolder,
    &MCS_GetVariable,
    &MCS_List_Create,
    &MCS_List_Set,
    &MCS_SetVariable,

    // mem
    &Mem_Free,
    &Mem_Malloc,
    &Mem_Memcpy,
    &Mem_Memset,

    // serial
    &Serial_Open,
    &Serial_Close,
    &Serial_ReadSingle,
    &Serial_Read,
    &Serial_Peek,
    &Serial_WriteSingle,
    &Serial_WriteUnbuffered,
    &Serial_Write,
    &Serial_PollRX,
    &Serial_PollTX,
    &Serial_ClearRX,
    &Serial_ClearTX,
    &Serial_IsOpen,

    // string
    &String_Strcat,
    &String_Strchr,
    &String_Strcmp,
    &String_Strcpy,
    &String_Strlen,
};

static const auto safe_guard = reinterpret_cast<const char *>(0x814fffe0);
static constexpr std::size_t safe_guard_size = 16;

static void ** find_symbol(const char *name) {
    for (auto element : symbols)
        if (std::strcmp(name, element->name) == 0)
            return &element->address;
    return nullptr;
}

extern "C" bool relink_sdk(const std::uint8_t * const mapping, const std::size_t size) {
    const auto new_safe_guard = reinterpret_cast<const char *>(mapping);

    if (std::strncmp(new_safe_guard, safe_guard, safe_guard_size) != 0)
        return false;

    auto ptr = mapping + std::strlen(safe_guard) + 1;

    while (ptr < mapping + size) {
        const auto name = reinterpret_cast<const char *>(ptr);
        ptr += std::strlen(name) + 1;

        std::uintptr_t address = 0;
        for (std::size_t i = 0; i < sizeof(decltype(address)) / sizeof(*ptr); i++, ptr++) {
            address <<= sizeof(*ptr) * 8;
            address |= *ptr;
        }

        const auto symbol = find_symbol(name);
        if (symbol == nullptr)
            continue;

        *symbol = reinterpret_cast<void *>(address);
    }
    return true;
}

extern "C" __attribute__((constructor(0))) void relocate_sdk() {
    const auto env_var = std::getenv("HHK_SYMBOL_TABLE");
    if (env_var == nullptr)
        return;
    const auto mapping = reinterpret_cast<const std::uint8_t *>(std::strtoul(env_var, nullptr, 16));
    if (mapping == nullptr)
        return;

    const auto len_env_var = std::getenv("HHK_SYMBOL_TABLE_LEN");
    auto len = std::strtoul(len_env_var, nullptr, 16);
    if (len == 0)
        return;

    if (!relink_sdk(mapping, len))
        return;
}

#include "sdk/calc/calc.h"

// deprecated to diagnose usages
#define PULL_REF(x) static __attribute__((used, deprecated)) auto ref_##x = &x;

PULL_REF(calcInit)
PULL_REF(calcExit)
