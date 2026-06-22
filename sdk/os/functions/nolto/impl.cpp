#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

typedef void * symbol_t;

struct symbol_entry {
    symbol_t * address;
    const char * const name;
};

#define REGISTER_SYMBOL(name) auto name = reinterpret_cast<symbol_t>(0xDEAD0001 + __LINE__ * 2);
#include "symbols.def"
#undef REGISTER_SYMBOL

#define REGISTER_SYMBOL(name) { &name, #name },
static const struct symbol_entry symbols[] = {
#include "symbols.def"
};
#undef REGISTER_SYMBOL

static const auto safe_guard = reinterpret_cast<const char *>(0x814fffe0);
static constexpr std::size_t safe_guard_size = 16;

static symbol_t * find_symbol(const char *name) {
    for (const auto &element : symbols)
        if (std::strcmp(name, element.name) == 0)
            return element.address;
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

        *symbol = reinterpret_cast<symbol_t>(address);
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
