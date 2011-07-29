OUTFILES           = build/eve.o
TARGET             = build/eve
INSTALL            = /usr/bin/eve

SUCCESS_MSG        = '  [\e[32m DONE \e[0m]'

CC                 = gcc
CFLAGS             = -std=c99 -Wall -Wextra -g


all: build

build: $(TARGET)

$(TARGET): $(OUTFILES)
	@echo 'Building target:'
	@$(CC) $(CFLAGS) $^ -o $@
	@echo -e $(SUCCESS_MSG)

build/eve.o: src/eve.c
	@mkdir -p build/
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo 'Cleaning workspace:'
	@rm -rf build/
	@echo -e $(SUCCESS_MSG)

rebuild: clean build

install: build
	@echo 'Installing target:'
	@cp -f $(TARGET) $(INSTALL)
	@echo -e $(SUCCESS_MSG)

uninstall:
	@echo 'Uninstalling target:'
	@rm -f $(INSTALL)
	@echo -e $(SUCCESS_MSG)

