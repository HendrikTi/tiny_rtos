cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake")

project("cmake-stm32")

set(CMAKE_C_STANDARD "11")
enable_language(ASM)

set(linker_script "${CMAKE_CURRENT_LIST_DIR}/STM32F4.ld")
set(device_name "STM32F412")

set(c_compile_options
	# CPU specific
	"-mcpu=cortex-m4"
	"-mfloat-abi=soft"
	"-mthumb"
	# Non-CPU specific
	"--specs=nano.specs"
	"-ffunction-sections"
	"-fdata-sections"
	"-fstack-usage"
	# Other options
	"-g3"
	"-c"
	"-O0"
	"-Wall"
)
	
set(compile_definitions "${device_name}")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	list(APPEND compile_definitions "DEBUG" "_DEBUG")
else()
	list(APPEND compile_definitions "NDEBUG")
endif()

set(link_flags
	"-mcpu=cortex-m0plus \
	-mfloat-abi=soft \
	-T\"${CMAKE_CURRENT_LIST_DIR}/STM32F4.ld\" \
	-Wl,-Map=\"${PROJECT_NAME}.map\" \
	-Wl,--gc-sections \
	-static \
	--specs=nano.specs \
	-Wl,--start-group -lc -lm -Wl,--end-group"
)
add_compile_options("${c_compile_options}")
add_compile_definitions("${compile_definitions}")
add_executable("${PROJECT_NAME}")
target_sources( "${PROJECT_NAME}" PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/startup_stm32f412cx.s ${CMAKE_CURRENT_LIST_DIR}/src/main.c)

set_target_properties("${PROJECT_NAME}" PROPERTIES LINK_FLAGS "${link_flags}")

# The OBJCOPY utility will separate the EEPROM section from the main program
add_custom_command(TARGET "${PROJECT_NAME}" POST_BUILD
	COMMAND "${CMAKE_OBJCOPY}" ARGS -j .eeprom "$<TARGET_FILE:${PROJECT_NAME}>" -O ihex "eeprom.hex"
	COMMAND "${CMAKE_OBJCOPY}" ARGS -R .eeprom "$<TARGET_FILE:${PROJECT_NAME}>" "program.elf"
	BYPRODUCTS "eeprom.hex" "program.elf"
)