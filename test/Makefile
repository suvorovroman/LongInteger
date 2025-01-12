TARGET_DIR := ..
TARGET_NAME := liblint.a
TARGET := $(TARGET_DIR)/$(TARGET_NAME)

# Каталог библиотечных файлов.
SRC_DIR := ../src
SRC	:= $(notdir $(shell find $(SRC_DIR) -name '*.c' -or -name '*.cpp'))

# Рабочий каталог для библиотечных файлов (объектные файлы и зависимости).
BUILD_DIR := ../build
OBJ	:= $(addprefix $(BUILD_DIR)/, $(SRC:%=%.o))
DEP	:= $(addprefix $(BUILD_DIR)/, $(SRC:%=%.d))

CFLAGS = -I$(SRC_DIR) -MMD -MP

all:	lt1

lt1:	lt1.c ../liblint.a
	$(CC) -I$(SRC_DIR) $< -llint -lm -L../ -o $@

lib:	$(TARGET)

$(TARGET):	$(OBJ)
	ar rcs $@ $(OBJ)

# Создание рабочего каталога перед компиляцией библиотеки.
$(OBJ): | $(BUILD_DIR)

$(filter %.c.o, $(OBJ)):$(BUILD_DIR)/%.c.o:$(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(filter %.cpp.o, $(OBJ)):$(BUILD_DIR)/%.cpp.o:$(SRC_DIR)/%.cpp
	$(CXX) -c $(CFLAGS) -o $@  $<

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -r -f ltest $(BUILD_DIR) $(TARGET) ./lt1

-include $(DEP)
