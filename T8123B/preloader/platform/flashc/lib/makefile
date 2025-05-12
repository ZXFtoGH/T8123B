###################################################################
# flag
###################################################################

MOD_SRC  = 
MOD_OBJ  = $(OBJ_PATH)

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC 		:= debug/debug.c
ifeq ($(DA_ARCH_AARCH64),1)
MOD_SRC 		+= heap/heap_wrapper.c
MOD_SRC 		+= heap/miniheap.c
MOD_SRC 		+= heap/page_alloc.c
else
MOD_SRC 		+= heap/heap.c
MOD_SRC 		+= libc/malloc.c
endif

MOD_SRC 		+= libc/printf.c
MOD_SRC 		+= libc/string/memset.c
MOD_SRC 		+= libc/string/memcmp.c
MOD_SRC 		+= libc/string/memcpy.c
MOD_SRC 		+= libc/string/memmove.c
MOD_SRC 		+= libc/string/memchr.c
MOD_SRC 		+= libc/string/strlcpy.c
MOD_SRC 		+= libc/string/strlen.c
MOD_SRC 		+= libc/string/atoi.c
MOD_SRC 		+= libc/string/rand.c

MOD_SRC 		+= libc/string/strnicmp.c
MOD_SRC 		+= libc/string/strcmp.c
MOD_SRC 		+= libc/string/strncmp.c
MOD_SRC 		+= libc/string/strncat.c
MOD_SRC 		+= libc/string/strncpy.c
MOD_SRC         += libc/string/strstr.c
MOD_SRC 		+= libc/string/ctype.c

MOD_SRC 		+= libc/string/strxfrm.c
MOD_SRC 		+= libc/string/strtok.c
MOD_SRC 		+= libc/string/strspn.c
MOD_SRC 		+= libc/string/strrchr.c
MOD_SRC         += libc/string/strpbrk.c
MOD_SRC 		+= libc/string/strnlen.c
MOD_SRC 		+= libc/string/strlcat.c
MOD_SRC 		+= libc/string/strdup.c
MOD_SRC 		+= libc/string/strcpy.c
MOD_SRC 		+= libc/string/strcoll.c
MOD_SRC 		+= libc/string/strchr.c
MOD_SRC 		+= libc/string/strtoul.c
MOD_SRC 		+= libc/string/strtol.c
MOD_SRC 		+= libc/string/strtod.c
MOD_SRC 		+= libc/string/strcat.c

MOD_SRC 		+= mxml/mxml-attr.c
MOD_SRC 		+= mxml/mxml-entity.c
MOD_SRC 		+= mxml/mxml-file.c
MOD_SRC 		+= mxml/mxml-get.c
MOD_SRC 		+= mxml/mxml-index.c
MOD_SRC 		+= mxml/mxml-node.c
MOD_SRC 		+= mxml/mxml-private.c
MOD_SRC 		+= mxml/mxml-search.c
MOD_SRC 		+= mxml/mxml-string.c
MOD_SRC 		+= mxml/mxml-set.c

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# include sub dir
###################################################################

include $(COMMON_DIR_MK)
