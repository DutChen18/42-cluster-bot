CFLAGS = -Wall -Wextra -std=c17 -pedantic -fsanitize=address -Og -g

srcdir = src
builddir = build
bin = cluster-bot
src = $(wildcard $(srcdir)/*.c)
obj = $(patsubst $(srcdir)/%.c,$(builddir)/%.o,$(src))
dep = $(patsubst $(srcdir)/%.c,$(builddir)/%.d,$(src))
inc = -I include

all: $(bin)

$(bin): $(obj)
	@mkdir -p $(builddir)
	$(CC) $(CFLAGS) $^ -o $@

$(builddir)/%.o: $(srcdir)/%.c
	@mkdir -p $(builddir)
	$(CC) $(CFLAGS) $< -o $@ -c -MMD $(inc)

clean:
	rm -rf $(builddir)
	rm -f $(bin)

re: clean
	make all

-include $(dep)
