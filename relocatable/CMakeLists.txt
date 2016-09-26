project(relocatable-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x1000 --code-loc 0xc050 --data-loc 0xd000 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms-ram.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_EXECUTABLE(rel-sample rel-sample.c)
ADD_DEPENDENCIES(rel-sample crt0-sms-ram)
TARGET_LINK_LIBRARIES(rel-sample console vdp)

ADD_EXECUTABLE(simple-bios simple-bios.c)
ADD_DEPENDENCIES(simple-bios crt0-sms-ram)
TARGET_LINK_LIBRARIES(simple-bios io)

ADD_EXECUTABLE(xmdump xmdump.c)
ADD_DEPENDENCIES(xmdump crt0-sms-ram)
TARGET_LINK_LIBRARIES(xmdump uart crc16-xmodem-bbb console vdp)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(rel-sample-bin ALL
        COMMAND makebin -s 53248 rel-sample.ihx rel-sample-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py rel-sample-raw.sms rel-sample.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py rel-sample.sms
		DEPENDS rel-sample
    )

ADD_CUSTOM_TARGET(simple-bios-bin ALL
        COMMAND makebin -s 53248 simple-bios.ihx simple-bios-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py simple-bios-raw.sms simple-bios.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py simple-bios.sms
		DEPENDS simple-bios
    )

ADD_CUSTOM_TARGET(xmdump-bin ALL
        COMMAND makebin -s 53248 xmdump.ihx xmdump-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py xmdump-raw.sms xmdump.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py xmdump.sms
		DEPENDS simple-bios
    )
