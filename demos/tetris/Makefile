APP_NAME:=tetris

ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4a_nofpueb-elf-gcc
AS_FLAGS:=-mrelax

FUNCTION_FLAGS=-flto -ffat-lto-objects -ffunction-sections -fdata-sections -mrelax -O2 -gdwarf-5

CC:=sh4a_nofpueb-elf-gcc
CC_FLAGS:=$(FUNCTION_FLAGS) -Wall -Wextra -I $(SDK_DIR)/include/

CXX:=sh4a_nofpueb-elf-g++
CXX_FLAGS:=$(FUNCTION_FLAGS) -Wall -Wextra -I $(SDK_DIR)/include/

LD:=sh4a_nofpueb-elf-g++
LD_FLAGS:=$(FUNCTION_FLAGS) -Wl,--gc-sections -L$(SDK_DIR)

READELF:=sh4a_nofpueb-elf-readelf
OBJCOPY:=sh4a_nofpueb-elf-objcopy
STRIP:=sh4a_nofpueb-elf-strip

AS_SOURCES:=$(wildcard *.S)
CC_SOURCES:=$(wildcard *.c)
CXX_SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(AS_SOURCES:.S=.o) $(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)

APP_ELF:=$(APP_NAME).elf
APP_OUT:=$(APP_NAME).hh3

elf: $(APP_ELF) Makefile
hh3: $(APP_OUT) Makefile
all: $(APP_ELF) $(APP_OUT) Makefile

clean:
	rm -f $(OBJECTS) $(APP_ELF) $(APP_ELF).map $(APP_OUT)

$(APP_ELF): $(OBJECTS) $(SDK_DIR)/libsdk.a
	$(LD) -Wl,-Map $@.map -o $@ $(LD_FLAGS) $(OBJECTS) -lsdk

$(APP_OUT): $(APP_ELF)
	$(STRIP) -o $@ $^

# We're not actually building libsdk.a, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/libsdk.a:
	echo "You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information" && exit 1

%.o: %.S
	$(AS) -c $< -o $@ $(AS_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

.PHONY: bin elf all clean
