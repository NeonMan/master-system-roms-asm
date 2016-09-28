project(relocatable-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x1000 --code-loc 0xc070 --data-loc 0xd800 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms-ram.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_EXECUTABLE(rel-sample rel-sample.c)
ADD_DEPENDENCIES(rel-sample crt0-sms-ram)
TARGET_LINK_LIBRARIES(rel-sample console vdp)

ADD_EXECUTABLE(simple-bios simple-bios.c)
ADD_DEPENDENCIES(simple-bios crt0-sms-ram)
TARGET_LINK_LIBRARIES(simple-bios io)

ADD_EXECUTABLE(xmdump xmdump.c)
ADD_DEPENDENCIES(xmdump crt0-sms-ram)
TARGET_LINK_LIBRARIES(xmdump uart crc16-xmodem console vdp)

ADD_EXECUTABLE(test-canary test-canary.c)
ADD_DEPENDENCIES(test-canary crt0-sms-ram)
TARGET_LINK_LIBRARIES(test-canary console vdp)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(rel-sample-bin ALL
        COMMAND makebin -s 57344 rel-sample.ihx rel-sample-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py rel-sample-raw.sms rel-sample.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py rel-sample.sms
		DEPENDS rel-sample
    )

ADD_CUSTOM_TARGET(simple-bios-bin ALL
        COMMAND makebin -s 57344 simple-bios.ihx simple-bios-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py simple-bios-raw.sms simple-bios.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py simple-bios.sms
		DEPENDS simple-bios
    )

ADD_CUSTOM_TARGET(xmdump-bin ALL
        COMMAND makebin -s 57344 xmdump.ihx xmdump-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py xmdump-raw.sms xmdump.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py xmdump.sms
		DEPENDS simple-bios
    )

ADD_CUSTOM_TARGET(test-canary-bin ALL
        COMMAND makebin -s 57344 test-canary.ihx test-canary-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py test-canary-raw.sms test-canary.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py test-canary.sms
		DEPENDS test-canary
    )
