#
# This is config used for compilation of Gumstix sources
#
# You might change location of U-Boot in memory by setting right TEXT_BASE.
# This allows for example having one copy located at the end of ram and stored
# in flash device and later on while developing use other location to test
# the code in RAM device only.
#

TEXT_BASE  = 0xA3F00000
ifeq ($(GUMSTIX_400MHZ),y)
	PLATFORM_CPPFLAGS += -DCONFIG_GUMSTIX_CPUSPEED_400
endif
PLATFORM_CPPFLAGS += -DSVN_REVISION='"$(SVN_REVISION)"'