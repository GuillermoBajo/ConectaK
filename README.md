# Connect K Game Implementation in Embedded Systems

The project develops a strategic game system (Connect K) for embedded systems, using various peripherals and timers to interact with the processor and the user. The system features a user-friendly input/output graphical interface via UART0, buttons configured as external interrupt lines, and an efficient event scheduler.

## Technologies Used

- **C Programming Language** for developing modules and controlling peripherals.
- **Interrupt Controller (VIC)** for handling specific events and controlling the program flow.
- **Timers and Watchdog** to measure time, generate precise delays, and reset the processor in case of malfunction.
- **GPIO** for simulating devices like buttons and LEDs, providing input and output interfaces.
- **Event Scheduler** to manage system events and reduce energy consumption.

## Key Features

- **Interaction with peripherals** such as timers, buttons, and UART0.
- **Event management** through a FIFO event queue and scheduler to dequeue and process events.
- **Energy consumption reduction** with sleep modes and processor reactivation.
- **System calls** to enable/disable IRQ and FIQ interrupts, and other critical operations.
- **Modular design** of modules presented through interfaces defined in header files (.h).

## System Design

- The implementation of the game's automaton ensures proper execution and coordination of the system's various components.
- The system operates in user and supervisor modes, using SWI calls to perform system management operations.
- The modular approach allows low-level abstraction, facilitating interaction with the main program and enabling greater scalability and system maintainability.
