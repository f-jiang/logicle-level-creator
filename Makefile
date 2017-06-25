SRC_DIR=Debug
TEST_DIR=test

.PHONY: main main-clean test test-clean

all: main test
clean: main-clean test-clean

main:
	cd $(SRC_DIR) && $(MAKE)

main-clean:
	cd $(SRC_DIR) && $(MAKE) clean

test:
	cd $(TEST_DIR) && $(MAKE)

test-clean:
	cd $(TEST_DIR) && $(MAKE) clean

