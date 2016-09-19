project(comp-libs C)

ADD_LIBRARY(lz4 lz4.c lz4.h)

ADD_EXECUTABLE(lz4-sample lz4-sample.c)
ADD_DEPENDENCIES(lz4-sample crt0-sms)
TARGET_LINK_LIBRARIES(lz4-sample lz4)


#Make .bin ROMs from the .ihx files
ADD_CUSTOM_TARGET(lz4-sample-bin ALL
        COMMAND makebin -s 32768 lz4-sample.ihx lz4-sample.sms
		DEPENDS lz4-sample
    )
    