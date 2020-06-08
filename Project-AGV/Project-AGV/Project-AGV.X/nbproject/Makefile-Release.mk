#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Release.mk)" "nbproject/Makefile-local-Release.mk"
include nbproject/Makefile-local-Release.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Release
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../magneto.c ../main.c ../Motor.c ../pins.c ../twi.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/magneto.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/Motor.o ${OBJECTDIR}/_ext/1472/pins.o ${OBJECTDIR}/_ext/1472/twi.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/magneto.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/Motor.o.d ${OBJECTDIR}/_ext/1472/pins.o.d ${OBJECTDIR}/_ext/1472/twi.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/magneto.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/Motor.o ${OBJECTDIR}/_ext/1472/pins.o ${OBJECTDIR}/_ext/1472/twi.o

# Source Files
SOURCEFILES=../magneto.c ../main.c ../Motor.c ../pins.c ../twi.c

# Pack Options 
PACK_COMPILER_OPTIONS=-I "${DFP_DIR}/include"
PACK_COMMON_OPTIONS=-B "${DFP_DIR}/gcc/dev/atmega32u4"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Release.mk dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATmega32U4
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/magneto.o: ../magneto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/magneto.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/magneto.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/magneto.o.d" -MT "${OBJECTDIR}/_ext/1472/magneto.o.d" -MT ${OBJECTDIR}/_ext/1472/magneto.o  -o ${OBJECTDIR}/_ext/1472/magneto.o ../magneto.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Motor.o: ../Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Motor.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/Motor.o.d" -MT "${OBJECTDIR}/_ext/1472/Motor.o.d" -MT ${OBJECTDIR}/_ext/1472/Motor.o  -o ${OBJECTDIR}/_ext/1472/Motor.o ../Motor.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/pins.o: ../pins.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pins.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pins.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/pins.o.d" -MT "${OBJECTDIR}/_ext/1472/pins.o.d" -MT ${OBJECTDIR}/_ext/1472/pins.o  -o ${OBJECTDIR}/_ext/1472/pins.o ../pins.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/twi.o: ../twi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4" -g -DDEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/twi.o.d" -MT "${OBJECTDIR}/_ext/1472/twi.o.d" -MT ${OBJECTDIR}/_ext/1472/twi.o  -o ${OBJECTDIR}/_ext/1472/twi.o ../twi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/1472/magneto.o: ../magneto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/magneto.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/magneto.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/magneto.o.d" -MT "${OBJECTDIR}/_ext/1472/magneto.o.d" -MT ${OBJECTDIR}/_ext/1472/magneto.o  -o ${OBJECTDIR}/_ext/1472/magneto.o ../magneto.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/main.o.d" -MT "${OBJECTDIR}/_ext/1472/main.o.d" -MT ${OBJECTDIR}/_ext/1472/main.o  -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/Motor.o: ../Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/Motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Motor.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/Motor.o.d" -MT "${OBJECTDIR}/_ext/1472/Motor.o.d" -MT ${OBJECTDIR}/_ext/1472/Motor.o  -o ${OBJECTDIR}/_ext/1472/Motor.o ../Motor.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/pins.o: ../pins.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/pins.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/pins.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/pins.o.d" -MT "${OBJECTDIR}/_ext/1472/pins.o.d" -MT ${OBJECTDIR}/_ext/1472/pins.o  -o ${OBJECTDIR}/_ext/1472/pins.o ../pins.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1472/twi.o: ../twi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/twi.o 
	 ${MP_CC}  $(MP_EXTRA_CC_PRE) -mmcu=atmega32u4  -I "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/include" -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4"  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -DF_CPU=16000000 -DNDEBUG  -I "../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wall -MD -MP -MF "${OBJECTDIR}/_ext/1472/twi.o.d" -MT "${OBJECTDIR}/_ext/1472/twi.o.d" -MT ${OBJECTDIR}/_ext/1472/twi.o  -o ${OBJECTDIR}/_ext/1472/twi.o ../twi.c  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver/leonardoe_usb_driver.X/dist/Release/debug/leonardoe_usb_driver.X.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega32u4 -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4"   -gdwarf-2 -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="dist\${CND_CONF}\${IMAGE_TYPE}\Project-AGV.X.${IMAGE_TYPE}.map"    -o dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\Thirdparty\Leonardo_AVRC\leonardoe_usb_driver\leonardoe_usb_driver.X\dist\Release\debug\leonardoe_usb_driver.X.a  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1  -L"A:/Users/Wouter/Documents/!Documenten/!School/HH/2019-2020/Semester 2/Project AGV/Project-AGV/Project-AGV/Project-AGV/Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group 
	
	${MP_CC_DIR}\\avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}\\avr-objdump -h -S "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}\\avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.srec"
	
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver/leonardoe_usb_driver.X/dist/Release/production/leonardoe_usb_driver.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mmcu=atmega32u4 -B "C:/Program Files (x86)/Atmel/Studio/7.0/Packs/Atmel/ATmega_DFP/1.4.351/gcc/dev/atmega32u4"  -D__$(MP_PROCESSOR_OPTION)__  -Wl,-Map="dist\${CND_CONF}\${IMAGE_TYPE}\Project-AGV.X.${IMAGE_TYPE}.map"    -o dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\Thirdparty\Leonardo_AVRC\leonardoe_usb_driver\leonardoe_usb_driver.X\dist\Release\production\leonardoe_usb_driver.X.a  -DXPRJ_Release=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)  -L"A:/Users/Wouter/Documents/!Documenten/!School/HH/2019-2020/Semester 2/Project AGV/Project-AGV/Project-AGV/Project-AGV/Thirdparty/Leonardo_AVRC/leonardoe_usb_driver" -Wl,--gc-sections -Wl,--start-group  -Wl,-lm -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.hex"
	${MP_CC_DIR}\\avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.eep" || exit 0
	${MP_CC_DIR}\\avr-objdump -h -S "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" > "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.lss"
	${MP_CC_DIR}\\avr-objcopy -O srec -R .eeprom -R .fuse -R .lock -R .signature "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "dist/${CND_CONF}/${IMAGE_TYPE}/Project-AGV.X.${IMAGE_TYPE}.srec"
	
	
endif


# Subprojects
.build-subprojects:
	cd ../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver/leonardoe_usb_driver.X && ${MAKE}  -f Makefile CONF=Release


# Subprojects
.clean-subprojects:
	cd ../Thirdparty/Leonardo_AVRC/leonardoe_usb_driver/leonardoe_usb_driver.X && rm -rf "build/Release" "dist/Release"

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} -r dist/Release

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
