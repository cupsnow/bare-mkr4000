# bare-mkr4000
## Motivation
*  Clean source tree target on mkr4000 😝.

----
## Build
*  Dependencies.
   *  Arduino mkr4000.
   *  CMSIS.
*  Modify Makefile as need.
   *  Path to the dependencies.
*  Targets
   *  bootloader -- Arduino bootloader.
   *  test1 -- Host application to test function, syntax ...
   *  test2 -- Target application based on Arduino library.
   *  test3 -- Target application to motivation.

----
## Note
*  $(PROJDIR)/moss -- Generic functions.
*  $(PROJDIR)/cmsis-mkr4000 -- Functions depends on CMSIS.
*  $(PROJDIR)/test/openocd* -- OpenOCD script on RPI, useful to write bootloader.
