TARGET = lib/libHelper.a

LIBDIR = lib/
INCDIR = include/

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP -I$(INCDIR) 
LDFLAGS =

dev: CFLAGS += $(DEV_CFLAGS)
dev: LDFLAGS += $(DEV_LDFLAGS)
dev: $(TARGET)

tsan: CFLAGS += $(TSAN_CFLAGS)
tsan: LDFLAGS += $(TSAN_LDFLAGS)
tsan: $(TARGET)

prod: CFLAGS += $(PROD_CFLAGS)
prod: LDFLAGS += $(PROD_LDFLAGS)
prod: $(TARGET)

$(TARGET): helpFuncs.o binaryWriter.o list.o include/helper.h | $(LIBDIR)
	@echo -e "\n"
	ar rs $(TARGET) helpFuncs.o binaryWriter.o list.o

helpFuncs.o: helpFuncs.c $(INCDIR)helpFuncs.h
	gcc $(CFLAGS) -c helpFuncs.c -o $@

binaryWriter.o: $(INCDIR)binaryWriter.h binaryWriter.c
	gcc $(CFLAGS) -c binaryWriter.c -o $@

list.o:list.c $(INCDIR)list.h  $(INCDIR)sortedList.h sortedList.c
	gcc $(CFLAGS) -c list.c -o $@

$(LIBDIR):
	mkdir -p $(LIBDIR)

# tools
clean:
	rm -f *.o *.d

fclean:
	rm -f *.o *.d $(TARGET) 

# merges .d files into dependency graph
-include *.d

