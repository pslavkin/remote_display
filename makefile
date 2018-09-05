#
# The base directory.
#
ROOT=.

#
# Include the common make definitions.
#
include ${ROOT}/tools/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH =./cmsis 
VPATH+=./board
VPATH+=./drivers
VPATH+=./flash
VPATH+=./c
VPATH+=./pic
VPATH+=./pic/font_30x42
VPATH+=./pic/font_22x30
VPATH+=./startup
VPATH+=./utilities

# Additional Compiler Flags
CFLAGS+=-D__USE_CMSIS
CFLAGS+=-D__USES_INITFINI__
CFLAGS+=-D PRINTF_FLOAT_ENABLE=1
CFLAGS+=-D PRINTF_ADVANCED_ENABLE
CFLAGS+=-D SCANF_FLOAT_ENABLE
CFLAGS+=-D __USE_CMSIS
CFLAGS+=-D CR_INTEGER_PRINTF
CFLAGS+=-D SDK_DEBUGCONSOLE=1
CFLAGS+=-D __MCUXPRESSO	
CFLAGS+=-D NDEBUG
CFLAGS+=-D SDK_OS_BAREMETAL
CFLAGS+=-D FSL_RTOS_BM
CFLAGS+=-D CPU_MKE18F512VLL16
CFLAGS+=-D CPU_MKE18F512VLL16_cm4
CFLAGS+=-D DISABLE_WDOG

# Where to find header files that do not live in the source directory.
IPATH= ./cmsis 
IPATH+=./board
IPATH+=./drivers
IPATH+=./flash
IPATH+=./h
IPATH+=./pic
IPATH+=./pic/font_30x42
IPATH+=./pic/font_22x30
IPATH+=./startup
IPATH+=./utilities
#
# The default rule, which causes the driver library to be built.
#
all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/app.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -f ${OBJDIR}/*
	@rm -f ${BINDIR}/*
#
#
# Rules for building the app
#
#../c
${BINDIR}/app.axf: ${OBJDIR}/main.o
${BINDIR}/app.axf: ${OBJDIR}/events.o
${BINDIR}/app.axf: ${OBJDIR}/state_machine.o
${BINDIR}/app.axf: ${OBJDIR}/everythings.o
${BINDIR}/app.axf: ${OBJDIR}/rti.o
${BINDIR}/app.axf: ${OBJDIR}/serial_session.o
${BINDIR}/app.axf: ${OBJDIR}/serial_tx_fifo.o
${BINDIR}/app.axf: ${OBJDIR}/serial_rx.o
${BINDIR}/app.axf: ${OBJDIR}/serial_tx.o
${BINDIR}/app.axf: ${OBJDIR}/serial_phisical.o
${BINDIR}/app.axf: ${OBJDIR}/str.o
${BINDIR}/app.axf: ${OBJDIR}/type_conversion.o
${BINDIR}/app.axf: ${OBJDIR}/schedule.o
${BINDIR}/app.axf: ${OBJDIR}/leds_session.o
${BINDIR}/app.axf: ${OBJDIR}/display_phisical.o
${BINDIR}/app.axf: ${OBJDIR}/display_pics.o
${BINDIR}/app.axf: ${OBJDIR}/display_layers.o
${BINDIR}/app.axf: ${OBJDIR}/buzzer.o
${BINDIR}/app.axf: ${OBJDIR}/tpm.o
${BINDIR}/app.axf: ${OBJDIR}/dma.o
${BINDIR}/app.axf: ${OBJDIR}/ftm.o
${BINDIR}/app.axf: ${OBJDIR}/tpanel.o
${BINDIR}/app.axf: ${OBJDIR}/adc.o
#../pics
${BINDIR}/app.axf: ${OBJDIR}/welcome_pic.o
${BINDIR}/app.axf: ${OBJDIR}/keyb_pic.o
${BINDIR}/app.axf: ${OBJDIR}/mask_pic.o
${BINDIR}/app.axf: ${OBJDIR}/pass_pic.o
${BINDIR}/app.axf: ${OBJDIR}/accept_reject_pic.o
${BINDIR}/app.axf: ${OBJDIR}/bkgd_pic.o
${BINDIR}/app.axf: ${OBJDIR}/log_pic.o
${BINDIR}/app.axf: ${OBJDIR}/font_30x42.o
${BINDIR}/app.axf: ${OBJDIR}/font_22x30.o

#../board
#${BINDIR}/app.axf: ${OBJDIR}/board.o
${BINDIR}/app.axf: ${OBJDIR}/clock_config.o
#${BINDIR}/app.axf: ${OBJDIR}/peripherals.o
${BINDIR}/app.axf: ${OBJDIR}/pin_mux.o 
#../cmsis
${BINDIR}/app.axf: ${OBJDIR}/system_mke18f16.o 
#../drivers
${BINDIR}/app.axf: ${OBJDIR}/fsl_clock.o
#${BINDIR}/app.axf: ${OBJDIR}/fsl_common.o
#${BINDIR}/app.axf: ${OBJDIR}/fsl_flash.o
${BINDIR}/app.axf: ${OBJDIR}/fsl_gpio.o
#${BINDIR}/app.axf: ${OBJDIR}/fsl_lpuart.o
#${BINDIR}/app.axf: ${OBJDIR}/fsl_smc.o 
#../startup
${BINDIR}/app.axf: ${OBJDIR}/startup.o 
#../utilities
#${BINDIR}/app.axf: ${OBJDIR}/fsl_debug_console.o
#${BINDIR}/app.axf: ${OBJDIR}/fsl_io.o
#${BINDIR}/app.axf: ${OBJDIR}/fsl_log.o
#${BINDIR}/app.axf: ${OBJDIR}/fsl_str.o 
${BINDIR}/app.axf: ${OBJDIR}/fsl_bootloader_conf.o

#../flash
${BINDIR}/app.axf: ${OBJDIR}/fsl_ftfx_flexnvm.c




#
#SCATTERapp=app.ld
#ENTRY_app=ResetISR

#
# Include the automatically generated dependency files.

#ifneq (${MAKECMDGOALS},clean)
#-include ${wildcard ${COMPILER}/*.d} __dummy__
#endif
