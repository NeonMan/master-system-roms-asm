cmake_minimum_required (VERSION 2.8)
project(test-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x4000 --code-loc 0x0090 --data-loc 0xc000 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

#get current revision ID
execute_process(COMMAND hg identify OUTPUT_VARIABLE COMMIT_ID)
string(STRIP "${COMMIT_ID}" COMMIT_ID)
#string(SUBSTRING "${COMMIT_ID}" 1, -1 COMMIT_ID)

#Make some definitions available
ADD_DEFINITIONS(-DCOMPILER_ID="${CMAKE_C_COMPILER_ID}")
ADD_DEFINITIONS(-DEXTENDED_ASCII)
ADD_DEFINITIONS(-DCOMMIT_ID=\"${COMMIT_ID}\")
ADD_DEFINITIONS(-DPROGRAM_MEDIA=ROM_CARTRIDGE)

#Make includes available everywhere
include_directories(${PROJECT_SOURCE_DIR})

# Find python3
find_package(PythonInterp 3 REQUIRED)

#uncoment and properly set this param if detection fails.
#set(PYTHON_EXECUTABLE "C:/Python31/python.exe")

#Libraries
ADD_SUBDIRECTORY(sms)
ADD_SUBDIRECTORY(crc)
ADD_SUBDIRECTORY(bios-menu)
ADD_SUBDIRECTORY(relocatable)
ADD_SUBDIRECTORY(comp)
ADD_SUBDIRECTORY(mapper-abuse)

#ROM "executables" ;)
ADD_EXECUTABLE(empty empty.c)
ADD_DEPENDENCIES(empty crt0-sms)

ADD_EXECUTABLE(sample sample.c)
ADD_DEPENDENCIES(sample crt0-sms)
TARGET_LINK_LIBRARIES(sample sdsc emu-hooks )

ADD_EXECUTABLE(test-ram test-ram.c)
ADD_DEPENDENCIES(test-ram crt0-sms)
TARGET_LINK_LIBRARIES(test-ram sdsc)

ADD_EXECUTABLE(console-test console-test.c)
ADD_DEPENDENCIES(console-test crt0-sms)
TARGET_LINK_LIBRARIES(console-test vdp console)

ADD_EXECUTABLE(sys-info sys-info.c)
ADD_DEPENDENCIES(sys-info crt0-sms)
TARGET_LINK_LIBRARIES(sys-info sms vdp console)

ADD_EXECUTABLE(test-input test-input.c)
ADD_DEPENDENCIES(test-input crt0-sms vdp console)
TARGET_LINK_LIBRARIES(test-input sms vdp console)

ADD_EXECUTABLE(test-uart-rx test-uart-rx.c)
ADD_DEPENDENCIES(test-uart-rx crt0-sms)
TARGET_LINK_LIBRARIES(test-uart-rx sms vdp console uart)

ADD_EXECUTABLE(test-uart-tx test-uart-tx.c)
ADD_DEPENDENCIES(test-uart-tx crt0-sms)
TARGET_LINK_LIBRARIES(test-uart-tx sms vdp console uart)

ADD_EXECUTABLE(test-uart-echo test-uart-echo.c)
ADD_DEPENDENCIES(test-uart-echo crt0-sms)
TARGET_LINK_LIBRARIES(test-uart-echo sms vdp console uart)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(sample-bin ALL
        COMMAND makebin -s 32768 sample.ihx sample.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py sample.sms
		DEPENDS sample
    )
ADD_CUSTOM_TARGET(empty-bin ALL
        COMMAND makebin -s 32768 empty.ihx empty.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py empty.sms
		DEPENDS empty
    )
ADD_CUSTOM_TARGET(test-ram-bin ALL
        COMMAND makebin -s 32768 test-ram.ihx test-ram.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py test-ram.sms
		DEPENDS test-ram
    )
ADD_CUSTOM_TARGET(console-test-bin ALL
        COMMAND makebin -s 32768 console-test.ihx console-test.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py console-test.sms
		DEPENDS console-test
    )
ADD_CUSTOM_TARGET(sys-info-bin ALL
        COMMAND makebin -s 32768 sys-info.ihx sys-info.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py sys-info.sms
		DEPENDS sys-info
    )
ADD_CUSTOM_TARGET(test-input-bin ALL
        COMMAND makebin -s 32768 test-input.ihx test-input.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py test-input.sms
		DEPENDS test-input
    )
ADD_CUSTOM_TARGET(test-uart-rx-bin ALL
        COMMAND makebin -s 32768 test-uart-rx.ihx test-uart-rx.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py test-uart-rx.sms
		DEPENDS test-input
    )
ADD_CUSTOM_TARGET(test-uart-tx-bin ALL
        COMMAND makebin -s 32768 test-uart-tx.ihx test-uart-tx.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py test-uart-tx.sms
		DEPENDS test-input
    )
ADD_CUSTOM_TARGET(test-uart-echo-bin ALL
        COMMAND makebin -s 32768 test-uart-echo.ihx test-uart-echo.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py test-uart-echo.sms
		DEPENDS test-input
    )
    