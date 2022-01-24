DEFAULT_HOST!=../../scripts/default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../../scripts/target-triplet-to-arch.sh $(HOST)
 
CFLAGS?=-O2 -g
CPPFLAGS?=
CPPEXTRAFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -fstack-protector -O0
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
CPPEXTRAFLAGS:=$(CPPEXTRAFLAGS) -nostdlib -fno-exceptions -fno-rtti
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc
 
ARCHDIR=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/boot/boot.o \
kernel/boot/gdt/gdt.o \
kernel/boot/tss/tss_table.o \
kernel/boot/interrupts/idt.o \
kernel/boot/interrupts/irq.o \
kernel/boot/interrupts/isr.o \
kernel/boot/pic/pic.o \
kernel/boot/pit/pit.o \
kernel/boot/a20/a20_enabler.o \
kernel/boot/utils/globals.o \
kernel/boot/utils/helpers.o \
kernel/boot/physical_memory/PhysicalMemoryManager.o \
kernel/boot/virtual_memory/pte.o \
kernel/boot/virtual_memory/pde.o \
kernel/boot/virtual_memory/VirtualMemoryManager.o \
kernel/drivers/Keyboard/keyboard.o \
kernel/drivers/Keyboard/Layouts/international_layout.o \
kernel/drivers/FloppyDisk/FloppyDisk.o \
kernel/drivers/DMA/Dma.o \
kernel/filesystem/vfs/fsys.o \
kernel/filesystem/fat12/fat12.o \
kernel/cli/cli.o \
kernel/kernel.o \
kernel/boot/interrupts/irqs.o \

#CRTI_OBJ=$(ARCHDIR)/crti.o
#CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
#CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
#CRTN_OBJ=$(ARCHDIR)/crtn.o
 
OBJS=\
$(CRTI_OBJ) \
$(KERNEL_OBJS) \
$(CRTN_OBJ) \
 
LINK_LIST=\
$(LDFLAGS) \
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(KERNEL_OBJS) \
$(LIBS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \
 
.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S
 
all: perry.kernel
 
perry.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot perry.kernel

$(CRTBEGIN_OBJ) $(CRTEND_OBJ):
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.cpp.o:
	$(CPP) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CPPEXTRAFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f perry.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: perry.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp perry.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)