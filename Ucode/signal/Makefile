src = $(wildcard *.c)
target = $(patsubst %.c,%,$(src))

ALL:$(target)

%:%.c
	gcc $< -o $(target)
clean:
	-rm -rf $(target)
.PHONY:clean ALL