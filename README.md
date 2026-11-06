# Embedded Processors Labs

Selected course work by Li Lushi using a NUCLEO-L432KC board, Mbed C++, and ARM assembly exercises.

## Experiments

| Directory | Topics | Materials |
| --- | --- | --- |
| [Lab 1](lab01-led-and-debugging/) | Button-controlled LED brightness and debugging | Two programs, submitted report |
| [Lab 2](lab02-assembly-and-pwm/) | ARM arithmetic, PWM and Ticker | Two assembly exercises, one Mbed program, submitted report |
| [Lab 3](lab03-adxl345-spi/) | ADXL345 over SPI, acceleration readings and LED orientation display | Three programs, submitted report |
| [Lab 4](lab04-tmp102-i2c/) | TMP102 over I2C, averaging, shutdown and button-triggered readings | Five programs, two saved serial outputs, submitted report |

## Using the code

Each exercise is a separate program. Import **one** exercise's `main.cpp` into a compatible Mbed project configured for NUCLEO-L432KC. Do not build the entire collection as one executable. Assembly exercises use the ARMSim# conventions from the course and run separately from the board programs.

The C++ files were originally saved as `.txt`; their bytes are preserved and only their names and directory layout were changed. Original assembly, reports and serial output files are preserved without edits. Reports contain the original course worksheets and submitted answers, including historical statements and identifiers.

The original Mbed revision and complete build configuration were not retained. This archive has not been rebuilt or tested on hardware during repository preparation.

## Known limitations

- Lab 1 brightness control compares floating-point values with exactly zero or one, which can miss bounds; it is retained as submitted.
- Lab 2 assembly overwrites `r0` with the output handle after computing into `r0`; its print behavior should be checked against ARMSim# conventions before reuse.
- Lab 3 Task 4 contains a placeholder instead of sensor initialization. It is an incomplete exercise and must not be treated as a standalone working program. Tasks 3 and 5 contain actual initialization code.
- Lab 4 button-triggered sampling switches conversion modes and reads immediately; fresh-conversion timing and negative-temperature handling need hardware and datasheet review before reuse.
- Saved serial output and report claims describe the archived course work, not new measurements or independently reproduced performance.

## Scope and attribution

This is a course archive. Teaching material embedded in the submitted worksheets remains attributable to its original authors. No separate open-source license is granted. Lecture slides, exam papers, standalone reference documents, duplicate packages, compiled binaries, and local working notes are excluded.
