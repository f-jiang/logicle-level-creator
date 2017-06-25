CXX=g++
CXXFLAGS=-std=c++11

PROJ_SRC_DIR=../src

EXEC_FILE=test
PROJ_SRC_FILES=gameboard.cpp level.cpp level_pack.cpp
TEST_SRC_FILES=test.cpp
PROJ_OBJ_FILES=$(PROJ_SRC_FILES:%.cpp=%.o)
TEST_OBJ_FILES=$(TEST_SRC_FILES:%.cpp=%.o)

$(EXEC_FILE): $(PROJ_OBJ_FILES) $(TEST_OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

$(PROJ_OBJ_FILES): %.o: $(PROJ_SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $^ -c

.PHONY: clean
clean:
	rm $(PROJ_OBJ_FILES) $(TEST_OBJ_FILES) $(EXEC_FILE)

