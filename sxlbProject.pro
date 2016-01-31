TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    array_or.cpp \
    ckernel.cpp \
    convert.cpp \
    ctor.cpp \
    font.cpp \
    gdt.cpp \
    heap.cpp \
    hwaccess.cpp \
    idt.cpp \
    keyboard.cpp \
    memory.cpp \
    scanf.cpp \
    syscall.cpp \
    system.cpp \
    task.cpp \
    test.cpp \
    textmode.cpp \
    timer.cpp \
    video.cpp \
    paging.cpp \
    heap_he.cpp \
    src/hw/keyboard.cpp \
    src/idt/idt.cpp \
    src/memory/heap.cpp \
    src/memory/heap_he.cpp \
    src/memory/memory.cpp \
    src/memory/paging.cpp \
    src/system/hwaccess.cpp \
    src/system/syscall.cpp \
    src/system/system.cpp \
    src/task/task.cpp \
    src/time/timer.cpp \
    src/ui/textmode.cpp \
    src/ui/video.cpp \
    src/user/test.cpp \
    src/convert.cpp \
    src/ctor.cpp \
    src/font.cpp \
    src/gdt.cpp \
    src/sprintf.cpp \
    src/hw/hwaccess.cpp \
    src/idt/fault_handler.cpp \
    src/io/hwaccess.cpp \
    src/io/keyboard.cpp \
    src/user/console.cpp \
    src/ui/window.cpp \
    src/time/date.cpp \
    src/string.cpp \
    src/user/elf.cpp \
    src/io/syskey_handler.cpp \
    src/utils/array_or.cpp \
    src/utils/queue.cpp \
    src/kernel.cpp \
    src/fs/initrd.cpp \
    src/fs/vfs.cpp \
    src/utils/version.cpp \
    src/user/executable.cpp \
    src/modules/module.cpp \
    src/boot/multiboot.cpp

OTHER_FILES += \
    boot.asm \
    cpu.asm \
    flush.asm \
    idtList.asm \
    kernel.asm

HEADERS += \
    array_or.hpp \
    convert.hpp \
    font.hpp \
    gdt.hpp \
    heap.hpp \
    hwaccess.hpp \
    idt.hpp \
    keyboard.hpp \
    memory.hpp \
    scanf.hpp \
    sprintf.hpp \
    syscall.hpp \
    system.hpp \
    task.hpp \
    test.hpp \
    textmode.hpp \
    timer.hpp \
    types.hpp \
    video.hpp \
    paging.hpp \
    heap_he.hpp \
    src/hw/keyboard.hpp \
    src/idt/idt.hpp \
    src/memory/heap.hpp \
    src/memory/heap_he.hpp \
    src/memory/memory.hpp \
    src/memory/paging.hpp \
    src/system/hwaccess.hpp \
    src/system/syscall.hpp \
    src/system/system.hpp \
    src/task/task.hpp \
    src/time/timer.hpp \
    src/ui/textmode.hpp \
    src/ui/video.hpp \
    src/user/test.hpp \
    src/array_or.hpp \
    src/convert.hpp \
    src/font.hpp \
    src/gdt.hpp \
    src/sprintf.hpp \
    src/types.hpp \
    src/hw/hwaccess.hpp \
    src/ctor.hpp \
    src/idt/fault_handler.hpp \
    src/io/hwaccess.hpp \
    src/io/keyboard.hpp \
    src/user/console.hpp \
    src/ui/window.hpp \
    src/time/date.hpp \
    src/fs/fs.hpp \
    src/string.hpp \
    src/system/syscall_defs.hpp \
    src/user/elf.hpp \
    src/io/syskey_handler.hpp \
    src/utils/array_or.hpp \
    src/utils/queue.hpp \
    src/fs/initrd.hpp \
    src/fs/vfs.hpp \
    src/utils/version.hpp \
    src/user/executable.hpp \
    src/modules/module.hpp \
    src/boot/multiboot.hpp

