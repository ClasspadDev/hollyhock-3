#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define HOLLYHOCK_ELF_NOTE(type, str) \
    __attribute__ ((section(".note.hollyhock"), used, aligned(4))) \
        static const struct { \
            uint32_t namesz; \
            uint32_t descsz; \
            uint32_t _type; \
            char name[sizeof(#type) + (4 - sizeof(#type) % 4) % 4]; \
            char desc[sizeof(str) + (4 - sizeof(str) % 4) % 4]; \
        } hollyhock_elf_note##type = { \
            sizeof(#type), \
            sizeof(str), \
            1, \
            #type, \
            str, \
        };

#define APP_NAME(app_name) \
    HOLLYHOCK_ELF_NOTE(NAME, app_name)
#define APP_DESCRIPTION(app_description) \
    HOLLYHOCK_ELF_NOTE(DESCRIPTION, app_description)
#define APP_AUTHOR(app_author) \
    HOLLYHOCK_ELF_NOTE(AUTHOR, app_author)
#define APP_VERSION(app_version) \
    HOLLYHOCK_ELF_NOTE(VERSION, app_version)

#ifdef __cplusplus
}
#endif
