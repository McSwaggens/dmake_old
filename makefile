CC=g++
CFLAGS=-w 
SOURCE_DIRECTORY=./src/
OUTPUT_DIRECTORY=./bin/
BINARY_NAME=dmake

COLOR_ERROR=\033[1;31m
COLOR_OK=\033[1;32m
COLOR_WARNING=\033[1;33m
COLOR_WHITE=\033[1;11m
COLOR_CLEAR=\033[0m

compile:
	@echo "$(COLOR_WHITE)Cleaning bin directory$(COLOR_CLEAR)"
	@rm -rf $(OUTPUT_DIRECTORY)
	@mkdir $(OUTPUT_DIRECTORY)
	
	@echo "$(COLOR_OK)Compiling...$(COLOR_CLEAR)"
	@$(CC) $(SOURCE_DIRECTORY)*.h $(SOURCE_DIRECTORY)*.cpp $(CFLAGS) -o $(OUTPUT_DIRECTORY)$(BINARY_NAME)

install:
	@echo "$(COLOR_WHITE)Installing..."
	@sudo cp ./bin/dmake /usr/bin/dmake
	@echo "$(COLOR_OK)Done"

clean:
	rm -rf $(OUTPUT_DIRECTORY)
