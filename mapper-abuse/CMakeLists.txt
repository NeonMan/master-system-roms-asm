project(mapper-abuse C)
ENABLE_LANGUAGE(C)

ADD_EXECUTABLE(abuse abuse.c)
ADD_DEPENDENCIES(abuse crt0-sms)
TARGET_LINK_LIBRARIES(abuse console vdp uart)

#Make .bin ROMs from the .ihx files

ADD_CUSTOM_TARGET(abuse-bin ALL
	COMMAND makebin -s 57344 abuse.ihx abuse.sms
#	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/trim.py abuse-raw.sms abuse.sms 49152
	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/fix-checksum.py abuse.sms 8
	DEPENDS abuse
	)
