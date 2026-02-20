cc = gcc
target = physics

src_dir = src
obj_dir = build

src = $(wildcard $(src_dir)/*.c)
obj = $(patsubst $(src_dir)/%.c,$(obj_dir)/%.o,$(src))

cflags = -Wall -Wextra -std=c11 -O2
sdl_cflags = $(shell sdl2-config --cflags)
sdl_libs = $(shell sdl2-config --libs)

$(obj_dir)/%.o: $(src_dir)/%.c
	mkdir -p $(obj_dir)
	$(cc) $(cflags) $(sdl_cflags) -c $< -o $@

$(target): $(obj)
	$(cc) $(obj) -o $@ $(sdl_libs)

clean:
	rm -rf $(obj_dir) $(target)

.PHONY: clean