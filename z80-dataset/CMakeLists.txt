project(relocatable-roms C)
ENABLE_LANGUAGE(C)

#Configure SDCC to make z80 binaries
set(CMAKE_C_FLAGS "-mz80 --std-sdcc89 --xram-size 0x1000 --code-loc 0xc070 --data-loc 0xd800 --no-std-crt0 \"${CMAKE_BINARY_DIR}/sms/crt0-sms-ram.rel\"")
set(CMAKE_EXE_LINKER_FLAGS "-Wl -w")

ADD_EXECUTABLE(dataset-xor dataset-xor.c)
ADD_DEPENDENCIES(dataset-xor crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-xor uart vdp console)

ADD_EXECUTABLE(dataset-add dataset-add.c)
ADD_DEPENDENCIES(dataset-add crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-add uart vdp console)

ADD_EXECUTABLE(dataset-and dataset-and.c)
ADD_DEPENDENCIES(dataset-and crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-and uart vdp console)

ADD_EXECUTABLE(dataset-or dataset-or.c)
ADD_DEPENDENCIES(dataset-or crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-or uart vdp console)

ADD_EXECUTABLE(dataset-cp dataset-cp.c)
ADD_DEPENDENCIES(dataset-cp crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-cp uart vdp console)

ADD_EXECUTABLE(dataset-sub dataset-sub.c)
ADD_DEPENDENCIES(dataset-sub crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-sub uart vdp console)

ADD_EXECUTABLE(dataset-adc dataset-adc.c)
ADD_DEPENDENCIES(dataset-adc crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-adc uart vdp console)

ADD_EXECUTABLE(dataset-sbc dataset-sbc.c)
ADD_DEPENDENCIES(dataset-sbc crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-sbc uart vdp console)

ADD_EXECUTABLE(dataset-neg dataset-neg.c)
ADD_DEPENDENCIES(dataset-neg crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-neg uart vdp console)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(dataset-neg-bin ALL
        COMMAND makebin -s 57344 -p dataset-neg.ihx dataset-neg-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-neg-raw.sms dataset-neg.sms 49152
		DEPENDS dataset-neg
    )
    
ADD_CUSTOM_TARGET(dataset-sbc-bin ALL
        COMMAND makebin -s 57344 -p dataset-sbc.ihx dataset-sbc-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-sbc-raw.sms dataset-sbc.sms 49152
		DEPENDS dataset-sbc
    )

ADD_CUSTOM_TARGET(dataset-adc-bin ALL
        COMMAND makebin -s 57344 -p dataset-adc.ihx dataset-adc-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-adc-raw.sms dataset-adc.sms 49152
		DEPENDS dataset-adc
    )

ADD_CUSTOM_TARGET(dataset-xor-bin ALL
        COMMAND makebin -s 57344 -p dataset-xor.ihx dataset-xor-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-xor-raw.sms dataset-xor.sms 49152
		DEPENDS dataset-xor
    )

ADD_CUSTOM_TARGET(dataset-sub-bin ALL
        COMMAND makebin -s 57344 -p dataset-sub.ihx dataset-sub-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-sub-raw.sms dataset-sub.sms 49152
		DEPENDS dataset-sub
    )

ADD_CUSTOM_TARGET(dataset-cp-bin ALL
        COMMAND makebin -s 57344 -p dataset-cp.ihx dataset-cp-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-cp-raw.sms dataset-cp.sms 49152
		DEPENDS dataset-cp
    )
    
ADD_CUSTOM_TARGET(dataset-add-bin ALL
        COMMAND makebin -s 57344 -p dataset-add.ihx dataset-add-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-add-raw.sms dataset-add.sms 49152
		DEPENDS dataset-add
    )

ADD_CUSTOM_TARGET(dataset-and-bin ALL
        COMMAND makebin -s 57344 -p dataset-and.ihx dataset-and-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-and-raw.sms dataset-and.sms 49152
		DEPENDS dataset-and
    )

ADD_CUSTOM_TARGET(dataset-or-bin ALL
        COMMAND makebin -s 57344 -p dataset-or.ihx dataset-or-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-or-raw.sms dataset-or.sms 49152
		DEPENDS dataset-or
    )

# Dataset headers from CSV files

ADD_CUSTOM_TARGET(dataset-header-add ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_add
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-add.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-add.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-adc ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_adc
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-adc.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-adc.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-sub ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_sub
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-sub.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-sub.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-sbc ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_sbc
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-sbc.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-sbc.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-cp ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_cp
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-cp.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-cp.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-and ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_and
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-and.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-and.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-or ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_or
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-or.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-or.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-xor ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_xor
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-xor.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-xor.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-neg ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_neg
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-neg.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-neg.h
    )
    
ADD_CUSTOM_TARGET(dataset-headers ALL
        DEPENDS dataset-header-add
        DEPENDS dataset-header-adc
        DEPENDS dataset-header-sub
        DEPENDS dataset-header-sbc
        DEPENDS dataset-header-cp
        DEPENDS dataset-header-and
        DEPENDS dataset-header-or
        DEPENDS dataset-header-xor
        DEPENDS dataset-header-neg
    )