ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
OBJDIR = build
INCLUDES = include
SRC = src
LIB = lib
SHARED = shared

CC = cc
CFLAGS = -g #-Wall -Werror -Wextra
LIBFLAGS = $(CFLAGS) -fpic
INCLUDE_FLAGS = -I$(INCLUDES)

SRC_FILES = malloc.c free.c
LIB_FILES = put_utils.c ft_printf.c
SHARED_FILES = heap_pools.c memory.c globals.c allocation.c \
			debug/utils.c debug/chunk_print.c debug/print_tools.c \
			debug/pool_print.c
SOURCES =	$(addprefix $(SRC)/, $(SRC_FILES)) \
			$(addprefix $(LIB)/, $(LIB_FILES)) \
			$(addprefix $(SHARED)/, $(SHARED_FILES))
OBJECTS =	$(addprefix $(OBJDIR)/, $(SRC_FILES:.c=.o)) \
			$(addprefix $(OBJDIR)/, $(LIB_FILES:.c=.o)) \
			$(addprefix $(OBJDIR)/, $(SHARED_FILES:.c=.o))
TEST_DIR = tests
TEST_SRC = test.c
TEST_SOURCES = $(addprefix $(TEST_DIR)/, $(TEST_SRC))
TEST_OBJ = $(addprefix $(OBJDIR)/, $(TEST_SRC:.c=.o))


all: $(NAME)
#all: test

$(NAME): $(OBJECTS)
	@echo "Creating shared library"
	@$(CC) -shared -o $(NAME) $(OBJECTS)
	@echo "Done!"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	@$(CC) $(LIBFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

$(OBJDIR)/%.o: $(LIB)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	@$(CC) $(LIBFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

$(OBJDIR)/%.o: $(SHARED)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	@$(CC) $(LIBFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

clean:
	@rm -rf $(OBJDIR)

debug: CFLAGS += -g3
debug: fclean $(NAME)

fclean: clean
	@rm -f $(NAME)
	@rm -f malloc_test

re: fclean all

$(OBJDIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

test: $(NAME) $(TEST_OBJ)
	@echo "Compiling test executable and running it"
	@$(CC) $(CFLAGS) -o malloc_test $(TEST_OBJ) $(NAME) $(INCLUDE_FLAGS)
	@LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH LD_PRELOAD=./$(NAME) ./malloc_test # &> test.log

.PHONY: all clean fclean re test debug