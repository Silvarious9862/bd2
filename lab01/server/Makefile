# Значение переменной BUILD можно задать извне (по умолчанию debug)
BUILD ?= debug

# Каталоги
SRC_DIR := ./src
INC_DIR := ./include
OUT_DIR := ./out/$(BUILD)

# Имя бинарного файла для нашего приложения
TARGET := $(OUT_DIR)/hello_world

# Исходные файлы (пока только один)
SRCS := $(SRC_DIR)/hello_world.c

# Компилятор
CC := gcc

# Общие CFLAGS
COMMON_CFLAGS := -Wall -Wextra -std=c11 -pedantic -I$(INC_DIR) -lcjson

ifeq ($(BUILD), debug)
	CFLAGS := $(COMMON_CFLAGS) -g -O0 -fsanitize=address -fno-omit-frame-pointer -DDEBUG
else ifeq ($(BUILD), release)
	CFLAGS := $(COMMON_CFLAGS) -O2 -DNDEBUG
else
	$(error Unknown build type: $(BUILD))
endif

.PHONY: all clean debug release

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) -lpq

debug:
	$(MAKE) BUILD=debug all

release:
	$(MAKE) BUILD=release all

clean:
	rm -rf ./out
