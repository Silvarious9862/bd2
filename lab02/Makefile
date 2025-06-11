# Makefile для проекта переноса данных
# Каталоги
SRC_DIR	:= src
INCLUDE_DIR:= include
BUILD_DIR  := build
OUT_DIR	:= out

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
LFLAGS := -lpq -ldb

# Поиск исходных файлов
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# Соответствующие объектные файлы помещаются в BUILD_DIR
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Название итогового бинарного файла
TARGET := $(OUT_DIR)/transfer

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $(LFLAGS)
	@echo "Сборка завершена, файл: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(OUT_DIR)
	@echo "Очищено: $(BUILD_DIR) и $(OUT_DIR)"

# Цели для отдельных сборок
debug:
	$(MAKE) MODE=debug

release:
	$(MAKE) MODE=release

.PHONY: all clean debug release
