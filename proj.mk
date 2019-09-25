#------------------------------------
#
COMMA:=,
EMPTY:=#
SPACE:=$(empty) $(empty)

PWD:=$(abspath .)
PROJDIR?=$(abspath $(dir $(firstword $(wildcard $(addsuffix /proj.mk,projdir .)))))
BUILDDIR?=$(PWD)/build
DESTDIR?=$(PWD)/destdir

$(info proj.mk ... MAKECMDGOALS: $(MAKECMDGOALS), PWD: $(PWD), \
  PROJDIR: $(PROJDIR), PLATFORM: $(PLATFORM), \
  CROSS_COMPILE: $(CROSS_COMPILE))

#------------------------------------
#
C++ =$(CROSS_COMPILE)g++
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
RM=rm -rf
MKDIR=mkdir -p
CP=rsync -a --info=progress2
OBJDUMP=$(CROSS_COMPILE)objdump
OBJCOPY=$(CROSS_COMPILE)objcopy
NM=$(CROSS_COMPILE)nm
SIZE=$(CROSS_COMPILE)size

CC_TARGET_HELP=$(CC) $(PLATFORM_CFLAGS) $(PLATFORM_LDFLAGS) -Q --help=target

DEP=$(1).d
DEPFLAGS=-MM -MF $(call DEP,$(1)) -MT $(1)

#------------------------------------
# $(shell touch /tmp/dummy.h; $(CC_DUMP_DEFINED) /tmp/dummy.h)
#
CC_DUMP_DEFINED=$(CC) -dM -E
C++_DUMP_DEFINED=$(C++) -dM -E -x c++

#------------------------------------
# EXTRA_PATH+=$(TOOLCHAIN_PATH:%=%/bin) $(TEST26DIR:%=%/tool/bin)
# export PATH:=$(call ENVPATH,$(EXTRA_PATH))
#
ENVPATH=$(subst $(SPACE),:,$(call UNIQ,$1) $(PATH))

#------------------------------------
# $(info AaBbccXXDF TOLOWER: $(call TOLOWER,AaBbccXXDF))
# $(info AaBbccXXDF TOUPPER: $(call TOUPPER,AaBbccXXDF))
#
MAPTO=$(subst $(firstword $1),$(firstword $2),$(if $(firstword $1),$(call MAPTO,$(filter-out $(firstword $1),$1),$(filter-out $(firstword $2),$2),$3),$3))
UPPERCASECHARACTERS=A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
LOWERCASECHARACTERS=a b c d e f g h i j k l m n o p q r s t u v w x y z
TOLOWER=$(call MAPTO,$(UPPERCASECHARACTERS),$(LOWERCASECHARACTERS),$1)
TOUPPER=$(call MAPTO,$(LOWERCASECHARACTERS),$(UPPERCASECHARACTERS),$1)

#------------------------------------
# $(call UNIQ,b b a a) # -> b a
#
UNIQ=$(if $1,$(strip $(firstword $1) $(call UNIQ,$(filter-out $(firstword $1),$1))))

#------------------------------------
#
define BUILD1
$1+=$2
$(1)_OBJ_CPP+=$$(patsubst %,$$(BUILDDIR)/%.o,$$(filter %.cpp,$$($1)))
$(1)_OBJ_C+=$$(patsubst %,$$(BUILDDIR)/%.o,$$(filter %.c,$$($1)))
$(1)_OBJ_ASM+=$$(patsubst %,$$(BUILDDIR)/%.o,$$(filter %.S,$$($1)))
$(1)_OBJ_LIB+=$$(filter %.a %.o,$$($1))
$(1)_OBJ_LDSCRIPT+=$$(filter %.ld,$$($1))
$(1)_LDFLAGS+=$$($(1)_OBJ_LDSCRIPT:%=-T %)
BUILD1_OBJ_C+=$$($(1)_OBJ_C)
BUILD1_OBJ_CPP+=$$($(1)_OBJ_CPP)
BUILD1_OBJ_ASM+=$$($(1)_OBJ_ASM)

$$($(1)_APP) $$($(1)_LIB): BUILD1_CPPFLAGS+=$$($(1)_CPPFLAGS) 
$$($(1)_APP) $$($(1)_LIB): BUILD1_CFLAGS+=$$($(1)_CFLAGS)
$$($(1)_APP) $$($(1)_LIB): BUILD1_CXXFLAGS+=$$($(1)_CXXFLAGS)
$$($(1)_APP) $$($(1)_LIB): BUILD1_CC?=$$(or $$($(1)_CC),$$(CC))
$$($(1)_APP) $$($(1)_LIB): BUILD1_C++ ?=$$(or $$($(1)_C++),$$(C++))
$$($(1)_APP): BUILD1_LDFLAGS+=$$($(1)_LDFLAGS)
$$($(1)_LIB): BUILD1_ARFLAGS?=$$(or $$($(1)_ARFLAGS),rcs)
$$($(1)_LIB): BUILD1_AR?=$$(or $$($(1)_AR),$$(AR))
$1: $$($(1)_APP) $$($(1)_LIB)

$$($(1)_APP): $$($(1)_OBJ_C) $$($(1)_OBJ_CPP) $$($(1)_OBJ_ASM) | $$($(1)_OBJ_LIB)
	$$(MKDIR) $$(dir $$@)
	$$(if $$($(1)_OBJ_CPP),$$(BUILD1_C++),$$(BUILD1_CC)) -o $$@ \
	  -Wl,--start-group $$($(1)_OBJ_C) $$($(1)_OBJ_CPP) $$($(1)_OBJ_ASM) \
	  $$($(1)_OBJ_LIB) -Wl,--end-group $$(BUILD1_CPPFLAGS) \
	  $$(if $$($(1)_OBJ_CPP),$$(BUILD1_CXXFLAGS),$$(BUILD1_CFLAGS)) \
	  $$(BUILD1_LDFLAGS)

$$($(1)_LIB): $$($(1)_OBJ_C) $$($(1)_OBJ_CPP) $$($(1)_OBJ_ASM)
	$$(MKDIR) $$(dir $$@)
	$$(BUILD1_AR) $$(BUILD1_ARFLAGS) $$@ \
	  $$($(1)_OBJ_C) $$($(1)_OBJ_CPP) $$($(1)_OBJ_ASM)

$(1)_clean:
	$$(RM) $$($(1)_OBJ_C) $$($(1)_OBJ_CPP) $$($(1)_OBJ_ASM) $$($(1)_OBJ_LIB) \
	  $$(addsuffix $$(DEP),$$($(1)_OBJ_C) $$($(1)_OBJ_CPP))

-include $$(addsuffix $$(DEP),$$($(1)_OBJ_C) $$($(1)_OBJ_CPP))
endef

define BUILD1_COMPILE
$$(sort $$($(1)_OBJ_C) $$($(1)_OBJ_CPP) $$($(1)_OBJ_ASM)): $$(BUILDDIR)/%.o: %
	$$(MKDIR) $$(dir $$@)
	$$(if $$(filter %.cpp,$$<),$$(BUILD1_C++),$$(BUILD1_CC)) \
	  -c -o $$@ $$< $$(BUILD1_CPPFLAGS) \
	  $$(if $$(filter %.cpp,$$<),$$(BUILD1_CXXFLAGS),$$(BUILD1_CFLAGS))
	$$(if $$(filter %.cpp,$$<),$$(BUILD1_C++),$$(BUILD1_CC)) \
	  -E -o $$(call DEP,$$@) $$(call DEPFLAGS,$$@) $$< $$(BUILD1_CPPFLAGS) \
	  $$(if $$(filter %.cpp,$$<),$$(BUILD1_CXXFLAGS),$$(BUILD1_CFLAGS))
endef

#------------------------------------
# $(call BUILD2,<name>, <objgen>, <src>)
#
define BUILD2
$1+=$3
$(1)_OBJGEN+=$$(patsubst %,$$(BUILDDIR)/$(2)/%.o,$$(filter %.cpp %.c %.S,$$($1)))
$(1)_INOBJ+=$$(filter %.a %.o,$$($1))
$(1)_LDSCRIPT+=$$(filter %.ld,$$($1))
$(1)_LDFLAGS+=$$($(1)_LDSCRIPT:%=-T %)
$(2)_OBJGEN+=$$($(1)_OBJGEN)

$$($(1)_APP) $$($(1)_LIB): BUILD2_CPPFLAGS+=$$($(1)_CPPFLAGS) 
$$($(1)_APP) $$($(1)_LIB): BUILD2_CFLAGS+=$$($(1)_CFLAGS)
$$($(1)_APP) $$($(1)_LIB): BUILD2_CXXFLAGS+=$$($(1)_CXXFLAGS)
$$($(1)_APP) $$($(1)_LIB): BUILD2_CC?=$$(or $$($(1)_CC),$$(CC))
$$($(1)_APP) $$($(1)_LIB): BUILD2_C++ ?=$$(or $$($(1)_C++),$$(C++))
$$($(1)_APP): BUILD2_LDFLAGS+=$$($(1)_LDFLAGS)
$$($(1)_LIB): BUILD2_ARFLAGS?=$$(or $$($(1)_ARFLAGS),rcs)
$$($(1)_LIB): BUILD2_AR?=$$(or $$($(1)_AR),$$(AR))
$1: $$($(1)_APP) $$($(1)_LIB)

$$($(1)_APP): $$($(1)_OBJGEN) | $$($(1)_INOBJ)
	$$(MKDIR) $$(dir $$@)
	$$(or $$($(1)_LD),$$(if $$(filter %.cpp,$$($1)),$$(BUILD2_C++),$$(BUILD2_CC))) \
	  -o $$@ -Wl,--start-group $$($(1)_OBJGEN) $$($(1)_INOBJ) -Wl,--end-group \
	  $$(BUILD2_CPPFLAGS) $$(if $$(filter %.cpp,$$($1)),$$(BUILD2_CXXFLAGS),$$(BUILD2_CFLAGS)) \
	  $$(BUILD2_LDFLAGS)

$$($(1)_LIB): $$($(1)_OBJGEN)
	$$(MKDIR) $$(dir $$@)
	$$(BUILD2_AR) $$(BUILD2_ARFLAGS) $$@ $$($(1)_OBJGEN)

$(1)_clean:
	$$(RM) $$($(1)_OBJGEN) $$(addsuffix $$(DEP),$$($(1)_OBJGEN))

-include $$(addsuffix $$(DEP),$$($(1)_OBJGEN))
endef

define BUILD2_OBJGEN
$$(sort $$($(1)_OBJGEN)): $$(BUILDDIR)/$(1)/%.o: %
	$$(MKDIR) $$(dir $$@)
	$$(if $$(filter %.cpp,$$<),$$(BUILD2_C++),$$(BUILD2_CC)) \
	  -c -o $$@ $$< $$(BUILD2_CPPFLAGS) \
	  $$(if $$(filter %.cpp,$$<),$$(BUILD2_CXXFLAGS),$$(BUILD2_CFLAGS))
	$$(if $$(filter %.cpp,$$<),$$(BUILD2_C++),$$(BUILD2_CC)) \
	  -E -o $$(call DEP,$$@) $$(call DEPFLAGS,$$@) $$< $$(BUILD2_CPPFLAGS) \
	  $$(if $$(filter %.cpp,$$<),$$(BUILD2_CXXFLAGS),$$(BUILD2_CFLAGS))
endef

#------------------------------------
#------------------------------------
#------------------------------------
#------------------------------------
#------------------------------------

