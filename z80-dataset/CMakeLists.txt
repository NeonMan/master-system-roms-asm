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

ADD_EXECUTABLE(dataset-inc dataset-inc.c)
ADD_DEPENDENCIES(dataset-inc crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-inc uart vdp console)

ADD_EXECUTABLE(dataset-dec dataset-dec.c)
ADD_DEPENDENCIES(dataset-dec crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-dec uart vdp console)

ADD_EXECUTABLE(dataset-rla dataset-rla.c)
ADD_DEPENDENCIES(dataset-rla crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rla uart vdp console)

ADD_EXECUTABLE(dataset-rl dataset-rl.c)
ADD_DEPENDENCIES(dataset-rl crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rl uart vdp console)

ADD_EXECUTABLE(dataset-rlc dataset-rlc.c)
ADD_DEPENDENCIES(dataset-rlc crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rlc uart vdp console)

ADD_EXECUTABLE(dataset-rlca dataset-rlca.c)
ADD_DEPENDENCIES(dataset-rlca crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rlca uart vdp console)

ADD_EXECUTABLE(dataset-rra dataset-rra.c)
ADD_DEPENDENCIES(dataset-rra crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rra uart vdp console)

ADD_EXECUTABLE(dataset-rr dataset-rr.c)
ADD_DEPENDENCIES(dataset-rr crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rr uart vdp console)

ADD_EXECUTABLE(dataset-rrc dataset-rrc.c)
ADD_DEPENDENCIES(dataset-rrc crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rrc uart vdp console)

ADD_EXECUTABLE(dataset-rrca dataset-rrca.c)
ADD_DEPENDENCIES(dataset-rrca crt0-sms-ram)
TARGET_LINK_LIBRARIES(dataset-rrca uart vdp console)

#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(dataset-rrca-bin ALL
        COMMAND makebin -s 57344 -p dataset-rrca.ihx dataset-rrca-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rrca-raw.sms dataset-rrca.sms 49152
		DEPENDS dataset-rrca
    )
	
ADD_CUSTOM_TARGET(dataset-rrc-bin ALL
        COMMAND makebin -s 57344 -p dataset-rrc.ihx dataset-rrc-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rrc-raw.sms dataset-rrc.sms 49152
		DEPENDS dataset-rrc
    )
	
ADD_CUSTOM_TARGET(dataset-rr-bin ALL
        COMMAND makebin -s 57344 -p dataset-rr.ihx dataset-rr-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rr-raw.sms dataset-rr.sms 49152
		DEPENDS dataset-rr
    )
	
ADD_CUSTOM_TARGET(dataset-rra-bin ALL
        COMMAND makebin -s 57344 -p dataset-rra.ihx dataset-rra-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rra-raw.sms dataset-rra.sms 49152
		DEPENDS dataset-rra
    )
	
ADD_CUSTOM_TARGET(dataset-rlca-bin ALL
        COMMAND makebin -s 57344 -p dataset-rlca.ihx dataset-rlca-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rlca-raw.sms dataset-rlca.sms 49152
		DEPENDS dataset-rlca
    )
	
ADD_CUSTOM_TARGET(dataset-rlc-bin ALL
        COMMAND makebin -s 57344 -p dataset-rlc.ihx dataset-rlc-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rlc-raw.sms dataset-rlc.sms 49152
		DEPENDS dataset-rlc
    )
	
ADD_CUSTOM_TARGET(dataset-rl-bin ALL
        COMMAND makebin -s 57344 -p dataset-rl.ihx dataset-rl-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rl-raw.sms dataset-rl.sms 49152
		DEPENDS dataset-rl
    )
	
ADD_CUSTOM_TARGET(dataset-rla-bin ALL
        COMMAND makebin -s 57344 -p dataset-rla.ihx dataset-rla-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-rla-raw.sms dataset-rla.sms 49152
		DEPENDS dataset-rla
    )
	
ADD_CUSTOM_TARGET(dataset-inc-bin ALL
        COMMAND makebin -s 57344 -p dataset-inc.ihx dataset-inc-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-inc-raw.sms dataset-inc.sms 49152
		DEPENDS dataset-inc
    )
	
ADD_CUSTOM_TARGET(dataset-dec-bin ALL
        COMMAND makebin -s 57344 -p dataset-dec.ihx dataset-dec-raw.sms
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py dataset-dec-raw.sms dataset-dec.sms 49152
		DEPENDS dataset-dec
    )

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
	
ADD_CUSTOM_TARGET(dataset-header-dec ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_dec
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-dec.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-dec.h
    )
	
ADD_CUSTOM_TARGET(dataset-header-inc ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_inc
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-inc.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-inc.h
    )
	
ADD_CUSTOM_TARGET(dataset-header-rla ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rla
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rla.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rla.h
    )
	
ADD_CUSTOM_TARGET(dataset-header-rl ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rl
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rl.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rl.h
    )
	
ADD_CUSTOM_TARGET(dataset-header-rlc ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rlc
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rlc.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rlc.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-rlca ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rlca
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rlca.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rlca.h
    )
	
ADD_CUSTOM_TARGET(dataset-header-rra ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rra
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rra.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rra.h
    )
	
ADD_CUSTOM_TARGET(dataset-header-rr ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rr
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rr.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rr.h
    )
	
ADD_CUSTOM_TARGET(dataset-header-rrc ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rrc
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rrc.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rrc.h
    )
    
ADD_CUSTOM_TARGET(dataset-header-rrca ALL
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/csv-to-header.py 
            z80_dataset_rrca
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rrca.csv
            ${CMAKE_SOURCE_DIR}/z80-dataset/dataset-rrca.h
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
		
		DEPENDS dataset-header-inc
		DEPENDS dataset-header-dec
		
		DEPENDS dataset-header-rla
		DEPENDS dataset-header-rl
		DEPENDS dataset-header-rlc
		DEPENDS dataset-header-rlca
		DEPENDS dataset-header-rra
		DEPENDS dataset-header-rr
		DEPENDS dataset-header-rrc
		DEPENDS dataset-header-rrca
    )