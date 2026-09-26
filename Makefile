TARGET = helpTest

LIBDIR = lib/
INCDIR = include/
SRCDIR = src/

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP -I$(INCDIR) -I$(SRCDIR)
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

$(TARGET): $(LIBDIR)libHelper.a main.o include/helper.h 
	gcc main.o -o $@ $(LDFLAGS) $(LIBDIR)libHelper.a -lm

$(LIBDIR)libHelper.a: helpFuncs.o binaryWriter.o list.o graph.o heap.o | $(LIBDIR)
	ar rs $@ $^

helpFuncs.o: $(SRCDIR)helpFuncs.c $(INCDIR)helpFuncs.h
	gcc $(CFLAGS) -c $(SRCDIR)helpFuncs.c -o $@

binaryWriter.o: $(INCDIR)binaryWriter.h $(SRCDIR)binaryWriter.c
	gcc $(CFLAGS) -c $(SRCDIR)binaryWriter.c -o $@

list.o:$(SRCDIR)list.c $(INCDIR)list.h  $(INCDIR)sortedList.h $(SRCDIR)sortedList.c
	gcc $(CFLAGS) -c $(SRCDIR)list.c -o $@

graph.o: $(SRCDIR)graph.c $(INCDIR)graph.h
	gcc $(CFLAGS) -c $(SRCDIR)graph.c -o $@

heap.o: $(SRCDIR)heap.c $(INCDIR)heap.h
	gcc $(CFLAGS) -c $< -o $@

$(LIBDIR):
	mkdir -p $(LIBDIR)

# tools
clean:
	rm -f *.o *.d

fclean:
	rm -f *.o *.d $(TARGET) $(LIBDIR)libHelper.a

# merges .d files into dependency graph
-include *.d

