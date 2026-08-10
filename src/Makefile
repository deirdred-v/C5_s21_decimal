FLAGS := -Wall -Werror -Wextra -std=c11
BUILD_DIR := ./build
CORE_DIR := ./core
CORE_SOURCE :=  $(shell find $(CORE_DIR) -type f -name "s21_*.c")
CORE_OBJECTS := $(patsubst $(CORE_DIR)/%.c, $(BUILD_DIR)/%.o, $(CORE_SOURCE))
TESTS_DIR := ./tests
TESTS_SOURCE := $(shell find $(TESTS_DIR) -type f -name "*.c")
TESTS_OBJECTS := $(patsubst $(TESTS_DIR)/%.c, $(BUILD_DIR)/tests/%.o, $(TESTS_SOURCE))
UNAME_S := $(shell uname -s)
WSL_DISTRO := $(shell wslpath -m / | awk -F'/' '{print $$4}')
LOGS_DIR := ./logs
PROJECT_PATH := $(abspath $(LOGS_DIR)/report.html)
REPORT_PATH := file://wsl.localhost/$(WSL_DISTRO)$(PROJECT_PATH)

ifeq ($(UNAME_S),Linux)
	OPEN_CMD = /mnt/c/Windows/explorer.exe
	CFLAGS = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
endif
ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	CFLAGS = -lcheck -lm
endif

all: clean s21_decimal.a

clean:
	rm -f s21_decimal.a
	rm -rf $(BUILD_DIR)
	rm -f $(CORE_DIR)/*.o
	rm -f ./*.out
	rm -f s21_decimal_test
	rm -rf $(LOGS_DIR)
	rm -f gcov_report
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete

test: $(CORE_OBJECTS) $(TESTS_OBJECTS)
	gcc $(FLAGS) $(CORE_OBJECTS) $(TESTS_OBJECTS) $(CFLAGS) -o s21_decimal_test
	./s21_decimal_test

$(BUILD_DIR)/%.o: $(CORE_DIR)/%.c 
	mkdir -p $(dir $@)
	gcc $(FLAGS) -c $< -o $@

$(BUILD_DIR)/tests/%.o: $(TESTS_DIR)/%.c
	mkdir -p $(dir $@)
	gcc $(FLAGS) -c $< -o $@

s21_decimal.a: $(CORE_OBJECTS)
	ar rc s21_decimal.a $^

gcov_html: | $(LOGS_DIR)
	-gcovr --html-details -o $(LOGS_DIR)/report.html \
		--html-title 'decimal.h' \
		--gcov-ignore-parse-errors=suspicious_hits.warn \
		--filter '$(abspath $(CORE_DIR))' \
		--exclude '$(abspath $(TESTS_DIR))' \
		--exclude '$(abspath $(BUILD_DIR))/tests' \
		$(BUILD_DIR)
ifneq ($(OPEN_CMD),)
	$(OPEN_CMD) $(REPORT_PATH) || true
endif

$(LOGS_DIR):
	@mkdir -p $(LOGS_DIR)

gcov_report: $(CORE_OBJECTS) $(TESTS_OBJECTS) | $(LOGS_DIR)
	gcc -fprofile-arcs -ftest-coverage $(CORE_OBJECTS) $(TESTS_OBJECTS) $(CFLAGS) -o gcov_report
	./gcov_report || true
	$(MAKE) gcov_html

.PHONY: all clean test gcov_report gcov_html s21_decimal.a 