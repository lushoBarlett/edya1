SRC   := src
TESTS := tests
BIN   := bin
BUILD := build
FLAGS := -Wall -Wextra -Werror -std=c99 -g

.PHONY: all
all: tests cbook

.PHONY: dirs
dirs:
	@mkdir -p $(BIN)
	@mkdir -p $(BUILD)



.PHONY: cbook
cbook: dirs $(BIN)/cbook

$(BIN)/cbook: $(BUILD)/main.o $(BUILD)/string.o $(BUILD)/csv.o $(BUILD)/contact_array.o $(BUILD)/algorithms.o \
              $(BUILD)/contact.o $(BUILD)/contact_book.o $(BUILD)/action.o $(BUILD)/action_stack.o
	$(CC) $(FLAGS) -o $@ $^

$(BUILD)/main.o: $(SRC)/main.c $(SRC)/string.h $(SRC)/csv.h $(SRC)/contact_book.h $(SRC)/action_stack.h
	$(CC) -c $(FLAGS) -o $@ $<



.PHONY: tests
tests: dirs $(BIN)/tests

$(BIN)/tests: $(BUILD)/tests.o $(BUILD)/test_string.o $(BUILD)/test_action_stack.o $(BUILD)/test_contact_book.o \
              $(BUILD)/test_contact_array.o $(BUILD)/test_action.o $(BUILD)/test_contact.o $(BUILD)/test_algorithms.o \
	      $(BUILD)/test_csv.o $(BUILD)/utils.o \
              $(BUILD)/string.o $(BUILD)/contact.o $(BUILD)/action.o $(BUILD)/algorithms.o \
              $(BUILD)/action_stack.o $(BUILD)/contact_book.o $(BUILD)/contact_array.o $(BUILD)/csv.o
	$(CC) $(FLAGS) -o $@ $^

$(BUILD)/tests.o: $(TESTS)/tests.c $(TESTS)/test_string.h $(TESTS)/test_action.h $(TESTS)/test_action_stack.h \
                  $(TESTS)/test_contact.h $(TESTS)/test_contact_array.h $(TESTS)/test_contact_book.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_string.o: $(TESTS)/test_string.c $(TESTS)/test_string.h $(SRC)/string.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_contact_book.o: $(TESTS)/test_contact_book.c $(TESTS)/test_contact_book.h $(SRC)/contact_book.h \
                              $(TESTS)/utils.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_contact_array.o: $(TESTS)/test_contact_array.c $(TESTS)/test_contact_array.h $(SRC)/contact_array.h \
                               $(TESTS)/utils.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_action_stack.o: $(TESTS)/test_action_stack.c $(TESTS)/test_action_stack.h $(SRC)/action_stack.h \
                              $(TESTS)/utils.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_contact.o: $(TESTS)/test_contact.c $(TESTS)/test_contact.h $(SRC)/contact.h \
                         $(TESTS)/utils.h
	$(CC) -c $(FLAGS) -o $@ $<
 
$(BUILD)/test_action.o: $(TESTS)/test_action.c $(TESTS)/test_action.h $(SRC)/action.h \
                        $(TESTS)/utils.h
	$(CC) -c $(FLAGS) -o $@ $<
 
$(BUILD)/test_algorithms.o: $(TESTS)/test_algorithms.c $(TESTS)/test_algorithms.h $(SRC)/algorithms.h \
                            $(TESTS)/utils.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/test_csv.o: $(TESTS)/test_csv.c $(TESTS)/test_csv.h $(SRC)/csv.h \
                     $(TESTS)/utils.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/utils.o: $(TESTS)/utils.c $(TESTS)/utils.h $(SRC)/contact.h
	$(CC) -c $(FLAGS) -o $@ $<

$(TEST)/utils.h: $(SRC)/contact.h $(SRC)/string.h



### CORE OBJECTS ###

$(BUILD)/string.o: $(SRC)/string.c $(SRC)/string.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/csv.o: $(SRC)/csv.c $(SRC)/csv.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/contact.o: $(SRC)/contact.c $(SRC)/contact.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/contact_array.o: $(SRC)/contact_array.c $(SRC)/contact_array.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/contact_book.o: $(SRC)/contact_book.c $(SRC)/contact_book.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/algorithms.o: $(SRC)/algorithms.c $(SRC)/algorithms.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/action.o: $(SRC)/action.c $(SRC)/action.h
	$(CC) -c $(FLAGS) -o $@ $<

$(BUILD)/action_stack.o: $(SRC)/action_stack.c $(SRC)/action_stack.h
	$(CC) -c $(FLAGS) -o $@ $<

#$(SRC)/action.h: $(SRC)/contact.h
#
#$(SRC)/contact.h: $(SRC)/string.h
#
#$(SRC)/contact_array.h: $(SRC)/contact.h
#
#$(SRC)/contact_book.h: $(SRC)/contact_array $(SRC)/contact.h
#
#$(SRC)/action_stack.h: $(SRC)/action.h
#
#$(SRC)/csv.h: $(SRC)/contact_array.h $(SRC)/string.h



.PHONY: clean
clean:
	@rm -r $(BIN)
	@rm -r $(BUILD)