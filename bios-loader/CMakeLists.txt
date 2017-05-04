project(bios-loader C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x1000 --code-loc 0xc070 --data-loc 0xd800 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms-ram.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_EXECUTABLE(bios-loader bios-loader.c)
ADD_DEPENDENCIES(bios-loader crt0-sms-ram)
TARGET_LINK_LIBRARIES(bios-loader uart vdp console io crc16-xmodem-bbb)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(bios-loader-bin ALL
        COMMAND makebin -p -s 57344 bios-loader.ihx bios-loader-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py bios-loader-raw.sms bios-loader.sms 49152
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py bios-loader-raw.sms 8
        DEPENDS bios-loader
    )
