project(relocatable-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x1000 --code-loc 0xc070 --data-loc 0xd800 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms-ram.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_EXECUTABLE(dataset-xor dataset-xor.c)
ADD_DEPENDENCIES(dataset-xor crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-xor uart vdp console)

ADD_EXECUTABLE(dataset-xor-ff dataset-xor-ff.c)
ADD_DEPENDENCIES(dataset-xor-ff crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-xor-ff uart vdp console)

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

ADD_EXECUTABLE(dataset-cp dataset-cp.c)
ADD_DEPENDENCIES(dataset-cp crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-cp uart vdp console)

ADD_EXECUTABLE(dataset-cp-ff dataset-cp-ff.c)
ADD_DEPENDENCIES(dataset-cp-ff crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-cp-ff uart vdp console)

ADD_EXECUTABLE(dataset-sub dataset-sub.c)
ADD_DEPENDENCIES(dataset-sub crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-sub uart vdp console)

ADD_EXECUTABLE(dataset-sub-ff dataset-sub-ff.c)
ADD_DEPENDENCIES(dataset-sub-ff crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-sub-ff uart vdp console)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(dataset-xor-bin ALL
        COMMAND makebin -s 57344 -p dataset-xor.ihx dataset-xor-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-xor-raw.sms dataset-xor.sms 49152
		DEPENDS dataset-xor
    )

ADD_CUSTOM_TARGET(dataset-xor-ff-bin ALL
        COMMAND makebin -s 57344 -p dataset-xor-ff.ihx dataset-xor-ff-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-xor-ff-raw.sms dataset-xor-ff.sms 49152
		DEPENDS dataset-xor-ff
    )
    
ADD_CUSTOM_TARGET(dataset-sub-bin ALL
        COMMAND makebin -s 57344 -p dataset-sub.ihx dataset-sub-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-sub-raw.sms dataset-sub.sms 49152
		DEPENDS dataset-sub
    )

ADD_CUSTOM_TARGET(dataset-sub-ff-bin ALL
        COMMAND makebin -s 57344 -p dataset-sub-ff.ihx dataset-sub-ff-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-sub-ff-raw.sms dataset-sub-ff.sms 49152
		DEPENDS dataset-sub-ff
    )

ADD_CUSTOM_TARGET(dataset-cp-bin ALL
        COMMAND makebin -s 57344 -p dataset-cp.ihx dataset-cp-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-cp-raw.sms dataset-cp.sms 49152
		DEPENDS dataset-cp
    )

ADD_CUSTOM_TARGET(dataset-cp-ff-bin ALL
        COMMAND makebin -s 57344 -p dataset-cp-ff.ihx dataset-cp-ff-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-cp-ff-raw.sms dataset-cp-ff.sms 49152
		DEPENDS dataset-cp-ff
    )
    
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
