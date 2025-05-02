DOXYGEN=doxygen

CC:=sh4a_nofpueb-elf-g++
CC_FLAGS=-flto -ffat-lto-objects -ffunction-sections -fdata-sections -Wall -Wextra -Wno-prio-ctor-dtor -pedantic -Werror -pedantic-errors -std=c++20 -gdwarf-5 -Os -mrelax -I include/

AR:=sh4a_nofpueb-elf-ar
AR_FLAGS:=src

CC_SOURCES:=$(shell find . -type f -name '*.cpp')
CC_NOLTO_SOURCES:=$(shell find . -type f -name '*.cpp' -path '*/nolto/*')
OBJECTS:=$(CC_SOURCES:.cpp=.o)
CC_NOLTO_OBJECTS:=$(CC_NOLTO_SOURCES:.cpp=.o)

all: libsdk.a Makefile

$(CC_NOLTO_OBJECTS): CC_FLAGS+=-fno-lto

docs:
	$(DOXYGEN)

clean:
	rm -f $(OBJECTS) libsdk.a

clean_docs:
	rm -rf doc/

libsdk.a: $(OBJECTS)

lib%.a:
	$(AR) $(AR_FLAGS) $@ $^

%.o: %.s
	$(AS) -c $< -o $@ $(AS_FLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CC_FLAGS)

.PHONY: all docs clean clean_docs
