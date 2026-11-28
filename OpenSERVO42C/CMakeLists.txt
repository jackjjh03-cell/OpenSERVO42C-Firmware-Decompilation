cmake_minimum_required(VERSION 3.16)
project(OpenSERVO42C_core C)
enable_testing()
add_library(openservo STATIC firmware/core/controller.c firmware/communication/protocol.c firmware/motion/motion.c firmware/core/pid.c firmware/hal/encoder.c firmware/hal/current.c firmware/hal/phase.c firmware/communication/stream.c firmware/core/drive.c firmware/hal/board.c firmware/communication/uart_transport.c firmware/hal/feedback_dma.c firmware/communication/bringup.c firmware/hal/output_gate.c firmware/core/run.c firmware/communication/run_protocol.c firmware/core/settings.c firmware/hal/settings_flash.c firmware/core/settings_session.c firmware/hal/calibration_check.c firmware/core/trace.c)
target_include_directories(openservo PUBLIC firmware/include)
target_compile_features(openservo PUBLIC c_std_11)
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(openservo PRIVATE -Wall -Wextra -Werror -Wconversion -Wsign-conversion)
endif()
add_executable(core_tests tests/core_tests.c)
target_link_libraries(core_tests PRIVATE openservo)
add_test(NAME core COMMAND core_tests)
find_package(Python3 3.8 COMPONENTS Interpreter REQUIRED)
add_test(NAME protocol_python COMMAND ${Python3_EXECUTABLE} -m unittest discover -s tests -p test_protocol.py)
set_tests_properties(protocol_python PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(motion_tests tests/motion_tests.c)
target_link_libraries(motion_tests PRIVATE openservo)
add_test(NAME motion_pid COMMAND motion_tests)

add_executable(integration_tests tests/integration_tests.c)
target_link_libraries(integration_tests PRIVATE openservo)
add_test(NAME controller_integration COMMAND integration_tests)
add_executable(wire_fixture tests/wire_fixture.c)
target_link_libraries(wire_fixture PRIVATE openservo)
add_test(NAME wire_roundtrip COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tests/wire_roundtrip.py" $<TARGET_FILE:wire_fixture>)
set_tests_properties(wire_roundtrip PROPERTIES TIMEOUT 15)

add_executable(encoder_tests tests/encoder_tests.c)
target_link_libraries(encoder_tests PRIVATE openservo)
add_test(NAME encoder COMMAND encoder_tests)

add_executable(current_tests tests/current_tests.c)
target_link_libraries(current_tests PRIVATE openservo)
add_test(NAME current_control COMMAND current_tests)

add_executable(phase_tests tests/phase_tests.c)
target_link_libraries(phase_tests PRIVATE openservo)
add_test(NAME electrical_phase COMMAND phase_tests)

add_executable(stream_tests tests/stream_tests.c)
target_link_libraries(stream_tests PRIVATE openservo)
add_test(NAME uart_stream COMMAND stream_tests)

add_executable(drive_tests tests/drive_tests.c)
target_link_libraries(drive_tests PRIVATE openservo)
add_test(NAME drive_integration COMMAND drive_tests)

add_executable(runtime_tests tests/runtime_tests.c firmware/target/runtime.c)
add_test(NAME runtime_arithmetic COMMAND runtime_tests)

add_executable(board_tests tests/board_tests.c)
target_link_libraries(board_tests PRIVATE openservo)
add_test(NAME board_startup COMMAND board_tests)

add_executable(uart_transport_tests tests/uart_transport_tests.c)
target_link_libraries(uart_transport_tests PRIVATE openservo)
add_test(NAME uart_transport COMMAND uart_transport_tests)

add_executable(feedback_dma_tests tests/feedback_dma_tests.c)
target_link_libraries(feedback_dma_tests PRIVATE openservo)
add_test(NAME feedback_dma COMMAND feedback_dma_tests)

add_executable(output_gate_tests tests/output_gate_tests.c)
target_link_libraries(output_gate_tests PRIVATE openservo)
add_test(NAME output_gate COMMAND output_gate_tests)

add_executable(run_tests tests/run_tests.c)
target_link_libraries(run_tests PRIVATE openservo)
add_test(NAME scheduler COMMAND run_tests)

add_executable(settings_tests tests/settings_tests.c)
target_link_libraries(settings_tests PRIVATE openservo)
add_test(NAME persistent_settings COMMAND settings_tests)

add_executable(settings_flash_tests tests/settings_flash_tests.c)
target_link_libraries(settings_flash_tests PRIVATE openservo)
add_test(NAME settings_flash COMMAND settings_flash_tests)

add_executable(settings_session_tests tests/settings_session_tests.c)
target_link_libraries(settings_session_tests PRIVATE openservo)
add_test(NAME settings_session COMMAND settings_session_tests)

add_test(NAME studio_client COMMAND ${Python3_EXECUTABLE} -m unittest discover -s tests -p test_studio_client.py)
set_tests_properties(studio_client PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(calibration_check_tests tests/calibration_check_tests.c)
target_link_libraries(calibration_check_tests PRIVATE openservo)
add_test(NAME calibration_check COMMAND calibration_check_tests)

add_executable(trace_tests tests/trace_tests.c)
target_link_libraries(trace_tests PRIVATE openservo)
add_test(NAME trace COMMAND trace_tests)
add_test(NAME studio_tuning COMMAND ${Python3_EXECUTABLE} -m unittest discover -s tests -p test_tuning.py)
set_tests_properties(studio_tuning PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_test(NAME studio_tuning_trials COMMAND ${Python3_EXECUTABLE} -m unittest discover -s tests -p test_tuning_trials.py)
set_tests_properties(studio_tuning_trials PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_test(NAME studio_trial_capture COMMAND ${Python3_EXECUTABLE} -m unittest discover -s tests -p test_trial_capture.py)
set_tests_properties(studio_trial_capture PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
