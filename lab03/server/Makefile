# Значение переменной BUILD можно задать извне (по умолчанию debug)
BUILD ?= debug

# Каталоги
SRC_DIR := ./src
INC_DIR := ./include
OUT_DIR := ./out/$(BUILD)

# Имя бинарного файла для нашего приложения
TARGET := $(OUT_DIR)/server_berkeley

# Исходные файлы – включаем все .c файлы в каталоге src
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Компилятор
CC := gcc

# Общие флаги компиляции (флаги для подключения заголовочных файлов)
COMMON_CFLAGS := -Wall -Wextra -std=c11 -pedantic -I$(INC_DIR) -D_DEFAULT_SOURCE

ifeq ($(BUILD), debug)
	CFLAGS := $(COMMON_CFLAGS) -g -O0 -fsanitize=address -fno-omit-frame-pointer -DDEBUG
else ifeq ($(BUILD), release)
	CFLAGS := $(COMMON_CFLAGS) -O2 -DNDEBUG
else
	$(error Unknown build type: $(BUILD))
endif

# Флаги линковки – подключаем PostgreSQL, cJSON и pthread
LDLIBS := -ldb -ljansson -lpthread

.PHONY: all clean debug release

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDLIBS)

debug:
	$(MAKE) BUILD=debug all

release:
	$(MAKE) BUILD=release all

clean:
	rm -rf ./out ./db_env
	rm *.log
