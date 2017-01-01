project(relocatable-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x1000 --code-loc 0xc070 --data-loc 0xd800 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms-ram.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_EXECUTABLE(dataset-add dataset-add.c)
ADD_DEPENDENCIES(dataset-add crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-add uart vdp console)

ADD_EXECUTABLE(dataset-add-ff dataset-add-ff.c)
ADD_DEPENDENCIES(dataset-add-ff crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-add-ff uart vdp console)

ADD_EXECUTABLE(dataset-and dataset-and.c)
ADD_DEPENDENCIES(dataset-and crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-and uart vdp console)

ADD_EXECUTABLE(dataset-and-ff dataset-and-ff.c)
ADD_DEPENDENCIES(dataset-and-ff crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-and-ff uart vdp console)

ADD_EXECUTABLE(dataset-or dataset-or.c)
ADD_DEPENDENCIES(dataset-or crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-or uart vdp console)

ADD_EXECUTABLE(dataset-or-ff dataset-or-ff.c)
ADD_DEPENDENCIES(dataset-or-ff crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-or-ff uart vdp console)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(dataset-add-bin ALL
        COMMAND makebin -s 57344 -p dataset-add.ihx dataset-add-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-add-raw.sms dataset-add.sms 49152
		DEPENDS dataset-add
    )

ADD_CUSTOM_TARGET(dataset-add-ff-bin ALL
        COMMAND makebin -s 57344 -p dataset-add-ff.ihx dataset-add-ff-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-add-ff-raw.sms dataset-add-ff.sms 49152
		DEPENDS dataset-add-ff
    )
    
ADD_CUSTOM_TARGET(dataset-and-bin ALL
        COMMAND makebin -s 57344 -p dataset-and.ihx dataset-and-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-and-raw.sms dataset-and.sms 49152
		DEPENDS dataset-and
    )

ADD_CUSTOM_TARGET(dataset-and-ff-bin ALL
        COMMAND makebin -s 57344 -p dataset-and-ff.ihx dataset-and-ff-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-and-ff-raw.sms dataset-and-ff.sms 49152
		DEPENDS dataset-and-ff
    )
    
ADD_CUSTOM_TARGET(dataset-or-bin ALL
        COMMAND makebin -s 57344 -p dataset-or.ihx dataset-or-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-or-raw.sms dataset-or.sms 49152
		DEPENDS dataset-or
    )

ADD_CUSTOM_TARGET(dataset-or-ff-bin ALL
        COMMAND makebin -s 57344 -p dataset-or-ff.ihx dataset-or-ff-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-or-ff-raw.sms dataset-or-ff.sms 49152
		DEPENDS dataset-or-ff
    )
