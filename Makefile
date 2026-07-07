TARGET = libHelper.a

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP
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

libHelper.a: helpFuncs.o binaryWriter.o list.o helper.h
	@echo "making helper library"
	ar rs libHelper.a helpFuncs.o binaryWriter.o list.o

helper.h: helpFuncs.o
	@echo "Generating portable helper.h"
	@echo "#pragma once" > helper.h
	@cat helpFuncs.h list.h intList.h sortedList.h binaryWriter.h atomicQueue.h >> helper.h

helpFuncs.o: helpFuncs.c helpFuncs.h
	gcc $(CFLAGS) -c helpFuncs.c -o $@

binaryWriter.o: binaryWriter.h binaryWriter.c
	gcc $(CFLAGS) -c binaryWriter.c -o $@

list.o:list.c list.h  sortedList.h sortedList.c
	gcc $(CFLAGS) -c list.c -o $@

# tools
clean:
	rm -f *.o *.d

fclean:
	rm -f *.o *.d helper.h $(TARGET) 

# merges .d files into dependency graph
-include *.d

