SRC   := src
TESTS := tests
BIN   := bin
BUILD := build
FLAGS := -Wall -Wextra -Werror -std=c99
LIBS  := -lm

.PHONY: all
all: tests expri

.PHONY: dirs
dirs:
	@mkdir -p $(BIN)
	@mkdir -p $(BUILD)



.PHONY: expri
expri: dirs $(BIN)/expri

$(BIN)/expri: $(BUILD)/main.o $(BUILD)/declarations.o $(BUILD)/dynamic_array.o \
              $(BUILD)/expression.o $(BUILD)/operators.o $(BUILD)/parser.o
	$(CC) $(FLAGS) $(LIBS) -o $@ $^

$(BUILD)/main.o: $(SRC)/main.c $(SRC)/parser.h $(SRC)/expression.h $(SRC)/operator_types.h \
		 $(SRC)/operators.h $(SRC)/declarations.h $(SRC)/dynamic_array.h 		              
	$(CC) -c $(FLAGS) -o $@ $<



.PHONY: tests
tests: dirs $(BIN)/tests

$(BIN)/tests: $(BUILD)/tests.o $(BUILD)/test_dynamic_array.o $(BUILD)/test_expression.o $(BUILD)/test_operators.o \
              $(BUILD)/test_declarations.o $(BUILD)/test_parser.o \
              $(BUILD)/dynamic_array.o $(BUILD)/expression.o $(BUILD)/operators.o \
              $(BUILD)/declarations.o $(BUILD)/parser.o
	$(CC) $(FLAGS) $(LIBS) -o $@ $^

$(BUILD)/tests.o: $(TESTS)/tests.c $(TESTS)/test_dynamic_array.h $(TESTS)/test_expression.h $(TESTS)/test_operators.h \
		  $(TESTS)/test_declarations.h $(TESTS)/test_parser.h 
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_dynamic_array.o: $(TESTS)/test_dynamic_array.c $(TESTS)/test_dynamic_array.h $(SRC)/dynamic_array.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_expression.o: $(TESTS)/test_expression.c $(TESTS)/test_expression.h $(SRC)/expression.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_operators.o: $(TESTS)/test_operators.c $(TESTS)/test_operators.h $(SRC)/operators.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_declarations.o: $(TESTS)/test_declarations.c $(TESTS)/test_declarations.h $(SRC)/declarations.h \
			      $(SRC)/dynamic_array.h $(SRC)/expression.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_parser.o: $(TESTS)/test_parser.c $(TESTS)/test_parser.h $(SRC)/parser.h $(SRC)/operator_types.h
	$(CC) -c $(FLAGS) -o $@ $<



### CORE OBJECTS ###

$(BUILD)/dynamic_array.o: $(SRC)/dynamic_array.c $(SRC)/dynamic_array.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/declarations.o: $(SRC)/declarations.c $(SRC)/declarations.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/expression.o: $(SRC)/expression.c $(SRC)/expression.h $(SRC)/operator_types.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/operators.o: $(SRC)/operators.c $(SRC)/operators.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/parser.o: $(SRC)/parser.c $(SRC)/parser.h $(SRC)/declarations.h $(SRC)/dynamic_array.h $(SRC)/expression.h 
	$(CC) -c $(FLAGS) -o $@ $<

$(SRC)/declarations.h: $(SRC)/expression.h $(SRC)/dynamic_array.h

$(SRC)/expression.h: $(SRC)/operator_types.h

$(SRC)/parser.h: $(SRC)/expression.h $(SRC)/dynamic_array.h



.PHONY: clean
clean:
	@rm -r $(BIN)
	@rm -r $(BUILD)