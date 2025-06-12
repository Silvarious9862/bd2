# Makefile для проекта переноса данных

# Каталоги
SRC_DIR	  := src
INCLUDE_DIR  := include
BUILD_DIR	:= build
OUT_DIR	  := out

# Компилятор
CC := gcc

# Режим сборки: debug или release (по умолчанию debug)
MODE ?= debug

ifeq ($(MODE), debug)
	CFLAGS := -Wall -Wextra -I$(INCLUDE_DIR) -g
else
	CFLAGS := -Wall -Wextra -I$(INCLUDE_DIR) -O2
endif

# Флаги для линковки
TRANSFER_LFLAGS   := -lpq -ldb
DB_INSPECT_LFLAGS := -ldb

# Исходные файлы для трансфера
TRANSFER_SRCS  := $(SRC_DIR)/main.c $(SRC_DIR)/db_migration.c $(SRC_DIR)/pg_migration.c
TRANSFER_OBJS  := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(TRANSFER_SRCS))
TRANSFER_BIN   := $(OUT_DIR)/transfer

# Исходный файл для утилиты db_inspect
DB_INSPECT_SRCS := $(SRC_DIR)/db_inspect.c
DB_INSPECT_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(DB_INSPECT_SRCS))
DB_INSPECT_BIN  := $(OUT_DIR)/db_inspect

# Цель all собирает программу трансфера по умолчанию
all: $(TRANSFER_BIN)

# Сборка программы трансфера
$(TRANSFER_BIN): $(TRANSFER_OBJS)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $(TRANSFER_OBJS) $(TRANSFER_LFLAGS)
	@echo "Сборка завершена, создан файл: $@"

# Сборка утилиты db_inspect
db_inspect: $(DB_INSPECT_BIN)

$(DB_INSPECT_BIN): $(DB_INSPECT_OBJS)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $(DB_INSPECT_OBJS) $(DB_INSPECT_LFLAGS)
	@echo "Сборка утилиты db_inspect завершена, создан файл: $@"

# Общее правило компиляции: объектные файлы в BUILD_DIR
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean очищает оба каталога : BUILD_DIR и OUT_DIR
clean:
	rm -rf $(BUILD_DIR) $(OUT_DIR) db_env
	@echo "Очищены каталоги: $(BUILD_DIR) и $(OUT_DIR)"

# Режимы сборки
debug:
	$(MAKE) MODE=debug

release:
	$(MAKE) MODE=release

.PHONY: all db_inspect clean debug release
