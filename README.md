# Digital Event Notice Board

A modular, bare-metal firmware system engineered for the **NXP ARM7 LPC2148 microcontroller**. The system combines real-time clock tracking with an asynchronous, non-blocking text-scrolling engine to manage time-critical scheduled notifications on a physical **16×2 HD44780 LCD**.

Rather than relying on traditional blocking delay routines that stall processor execution, this firmware uses dedicated vectored interrupts and state-machine scheduling to keep background data sampling fluid while scrolling text strings smoothly.

## Hardware Profile & Peripheral Mapping

**Microcontroller:** NXP ARM7 LPC2148 (Core frequency configured to **60 MHz via PLL0**; Peripheral Clock **VPB running at 15 MHz**).

**Display Layout:** 16×2 HD44780 LCD operating on an **8-bit parallel data bus** for low-latency display updates.

**User Input:** 4×4 Matrix Keypad mapped to dedicated GPIO rows and columns, using software debouncing to manage physical key-switch noise.

**Timing & Schedulers:** On-chip Hardware **Real-Time Clock (RTC)** for standalone calendar tracking, alongside **Timer0 Match Registers** for countdown calculations.

**Sensor Diagnostics:** 10-bit Successive Approximation on-chip **Analog-to-Digital Converter (ADC0, Channel 2)**.

## Core Firmware Engines

### 1. Text Scrolling Algorithm

Displaying strings longer than the 16-character hardware boundary requires a non-destructive memory sliding-window technique. The system handles this operation by mapping character strings onto the display grid step-by-step and shifting the base string memory pointer forward across the buffer array to create a continuous scrolling effect.

### 2. Asynchronous Interrupt Design (`__irq`)

To maximize efficiency, blocking loops are removed from the main routine, and time-critical operations are mapped to independent hardware Interrupt Service Routines (**ISRs**) through the **Vectored Interrupt Controller (VIC)**:

**`rtc_alarm_isr` (Slot 1):** Monitors scheduled events in the background every minute. When a matching schedule is detected, it asserts the `startScroll` flag and exits immediately to allow text processing.

**`eint0_isr` (Slot 0):** Mapped to an edge-sensitive, active-low external push button. Pressing the button interrupts standard execution and shifts the system into the interactive configuration menu.

**`Timer0_ISR` (Slot 2):** Configured with a Prescale Value of **14,999** to divide the **15 MHz peripheral clock** into a precise **1 kHz (1 ms) time base**. The Match Register reaches **1000 counts** to generate an exact **1-second timing pulse** for countdown tracking.

### 3. Non-Blocking Sensor Interleaving

Rather than stalling execution using a hard delay for sensor updates, the background firmware uses an automated time-slice verification condition (`sec % 2 == 0`). The ADC reads the analog input channel only during even seconds, keeping temperature and sensor diagnostics running seamlessly without causing stutter in the scrolling display.

## Repository Structure

The repository maintains all necessary project and source files directly in a flat directory layout for development and deployment:

**Source Files (`.c`) →** Implementation code including `eb_main.c`, `lcd.c`, `kpm.c`, `adc.c`, `lcd_scrolling.c`, `rtc_lcd.c`, `admin_mode.c`, `rtc_interrupt.c`, `ext_int0.c`, `timer_interrupt.c`, and `countdown.c`.

**Header Files (`.h`) →** Interface definitions and register mappings including `types.h`, `delay.h`, `global.h`, `msg.h`, `defines.h`, `lcd_defines.h`, `kpm_defines.h`, and `adc_defines.h`.

**`Startup.s` →** Target-specific ARM assembly startup file that configures stack pointers and handles interrupt vector table mapping before transferring execution to `main()`.

**`event_board_final.hex` →** The precompiled execution binary used for direct programming or loading onto the LPC2148 target hardware framework.

## Hardware Validation & Production States

**Standby Mode:** Displays real-time clock information, complete calendar date values, and continuous ambient sensor readings. A **Red Status LED** remains active to indicate that the system is waiting for a scheduled event.

**Active Notice Mode:** When the RTC time matches a scheduled event, the system automatically shifts states. A **Green Status LED** turns ON, the active notice message scrolls smoothly across **LCD Line 1**, and an active countdown timer is displayed on **LCD Line 2**.

**Admin Configuration Dashboard:** Triggered through the **EINT0 button**, allowing users to enter new RTC values, modify calendar configurations, and enable or disable specific scheduled message blocks using the **4×4 matrix keypad**.
