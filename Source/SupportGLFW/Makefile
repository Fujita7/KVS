#*****************************************************************************
#  $Id: Makefile 448 2010-01-05 09:51:35Z naohisa.sakamoto $
#*****************************************************************************

#=============================================================================
#  Include.
#=============================================================================
include ../../kvs.conf
include ../../Makefile.def


#=============================================================================
#  INCLUDE_PATH, LIBRARY_PATH, LINK_LIBRARY, INSTALL_DIR.
#=============================================================================
INCLUDE_PATH := -I.. $(GLFW_INCLUDE_PATH)
LIBRARY_PATH := $(GLFW_LIBRARY_PATH)
LINK_LIBRARY := $(GLFW_LINK_LIBRARY)
INSTALL_DIR  := $(KVS_DIR)


#=============================================================================
#  Include build rule.
#=============================================================================
include BuildRule.mk


#=============================================================================
#  Project name.
#=============================================================================
TARGET_LIB := $(OUTDIR)/$(LIB_KVS_SUPPORT_GLFW)


#=============================================================================
#  Build rule.
#=============================================================================
$(TARGET_LIB): $(OBJECTS)
	$(AR) $@ $^
	$(RANLIB) $@


#=============================================================================
#  build.
#=============================================================================
build:: $(TARGET_LIB)


#=============================================================================
#  clean.
#=============================================================================
clean::
	$(RMDIR) $(OUTDIR)


#=============================================================================
#  install.
#=============================================================================
install::
	$(MKDIR) $(INSTALL_DIR)/lib
	$(INSTALL) $(TARGET_LIB) $(INSTALL_DIR)/lib
	$(RANLIB) $(INSTALL_DIR)/lib/$(LIB_KVS_SUPPORT_GLFW)
