cmake_minimum_required(VERSION 3.21)

get_filename_component(CH592_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CH592_ROOT}/cmake/toolchain-ch59x.cmake")
endif()

project(CH592F_RGB_GPIO_HIGH_V2 C ASM)
set(TARGET_ELF ${PROJECT_NAME}.elf)

# Reuse the board configuration, startup, SDK and app flash address (0x1000).
# The normal keyboard entry point and WS2812 driver are deliberately excluded.
add_executable(${TARGET_ELF}
    main.c
    ${CH592_ROOT}/SDK/Startup/startup_CH592.S
    ${CH592_ROOT}/SDK/StdPeriphDriver/CH59x_clk.c
    ${CH592_ROOT}/SDK/StdPeriphDriver/CH59x_gpio.c
    ${CH592_ROOT}/SDK/StdPeriphDriver/CH59x_pwr.c
    ${CH592_ROOT}/SDK/StdPeriphDriver/CH59x_sys.c
    ${CH592_ROOT}/SDK/StdPeriphDriver/CH59x_timer1.c
)
target_include_directories(${TARGET_ELF} PRIVATE
    ${CH592_ROOT}/SDK/StdPeriphDriver/inc
    ${CH592_ROOT}/SDK/RVMSIS
    ${CH592_ROOT}/hal/include
)
target_compile_definitions(${TARGET_ELF} PRIVATE KBD_LAYOUT_5KEY FREQ_SYS=60000000)
target_compile_options(${TARGET_ELF} PRIVATE "$<$<COMPILE_LANGUAGE:C>:-std=gnu99;-Wall;-Wextra>")
target_link_options(${TARGET_ELF} PRIVATE
    -T${CH592_ROOT}/SDK/Ld/Link.ld
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map
)
target_link_libraries(${TARGET_ELF} PRIVATE ${CH592_ROOT}/SDK/StdPeriphDriver/libISP592.a)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CROSS_OBJCOPY} -Obinary $<TARGET_FILE:${TARGET_ELF}>
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-app.bin
    COMMAND ${CROSS_OBJCOPY} -Oihex $<TARGET_FILE:${TARGET_ELF}>
            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-app.hex
    COMMAND ${CROSS_SIZE} --format=berkeley $<TARGET_FILE:${TARGET_ELF}>
    VERBATIM
)
