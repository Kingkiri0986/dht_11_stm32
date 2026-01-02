# DHT11_STM32

A robust and easy-to-use library for interfacing the DHT11 temperature and humidity sensor with STM32 microcontrollers using HAL drivers.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![STM32](https://img.shields.io/badge/STM32-HAL-green.svg)
![Version](https://img.shields.io/badge/version-1.0.0-orange.svg)

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Hardware Setup](#hardware-setup)
- [Configuration](#configuration)
- [Usage](#usage)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## 🔍 Overview

This library provides a simple and efficient interface for reading temperature and humidity data from the DHT11 sensor using STM32 microcontrollers. It leverages STM32 HAL drivers and hardware timers to ensure accurate timing for the DHT11's single-wire communication protocol.

### DHT11 Sensor Specifications

| Parameter | Value |
|-----------|-------|
| Temperature Range | 0°C to 50°C |
| Temperature Accuracy | ±2°C |
| Humidity Range | 20% to 90% RH |
| Humidity Accuracy | ±5% RH |
| Sampling Rate | 1 Hz (1 reading/second) |
| Operating Voltage | 3.3V - 5.5V |
| Current Consumption | 0.5 - 2.5 mA |
| Interface | Single-wire digital |

## ✨ Features

- ✅ **HAL Driver Based** - Built on STM32 HAL for maximum portability
- ✅ **Hardware Timer Support** - Uses TIM1-TIM4 for precise microsecond timing
- ✅ **Easy Integration** - Simple API with minimal setup
- ✅ **Error Handling** - Comprehensive error detection and reporting
- ✅ **Checksum Verification** - Ensures data integrity
- ✅ **Temperature Conversion** - Supports Celsius and Fahrenheit
- ✅ **Lightweight** - Minimal memory footprint
- ✅ **Multiple STM32 Series** - Compatible with F0, F1, F4, L0, L4, etc.

## 🔧 Hardware Requirements

- STM32 microcontroller (any series with HAL support)
- DHT11 temperature and humidity sensor module
- 4.7kΩ or 10kΩ pull-up resistor (if not included on sensor module)
- Breadboard and jumper wires
- USB cable for programming/debugging

### Tested Boards

- ✔️ STM32F103C8T6 (Blue Pill)
- ✔️ STM32F411CEU6 (Black Pill)
- ✔️ STM32 Nucleo-F401RE
- ✔️ STM32 Nucleo-F411RE
- ✔️ STM32 Nucleo-L476RG
- ✔️ STM32F407VG Discovery

## 💻 Software Requirements

- **STM32CubeMX** - Version 6.0 or later
- **IDE** (choose one):
  - STM32CubeIDE (recommended)
  - Keil MDK-ARM
  - IAR Embedded Workbench
- **STM32 HAL Library** - Included with STM32CubeMX

## 📥 Installation

### Method 1: Clone Repository

```bash
git clone https://github.com/yourusername/dht11_stm32.git
cd dht11_stm32
```

### Method 2: Download ZIP

Download the latest release from the [Releases](https://github.com/yourusername/dht11_stm32/releases) page.

### Method 3: Add to Existing Project

1. Copy `dht11.h` and `dht11.c` to your project's `Inc` and `Src` folders
2. Add the files to your IDE project
3. Include the header in your main code: `#include "dht11.h"`

## 🔌 Hardware Setup

### Wiring Diagram

```
DHT11 Sensor          STM32 Board
┌──────────┐         ┌────────────┐
│          │         │            │
│   VCC    ├─────────┤ 3.3V/5V    │
│          │         │            │
│   DATA   ├────┬────┤ GPIO (e.g. PA0)
│          │    │    │            │
│          │   [R]   │            │ [R] = 4.7kΩ - 10kΩ
│          │    │    │            │ pull-up resistor
│   NC     │    └────┤ 3.3V/5V    │
│          │         │            │
│   GND    ├─────────┤ GND        │
│          │         │            │
└──────────┘         └────────────┘
```

### Connection Table

| DHT11 Pin | STM32 Pin | Description |
|-----------|-----------|-------------|
| Pin 1 (VCC) | 3.3V or 5V | Power supply |
| Pin 2 (DATA) | Any GPIO (e.g., PA0) | Data line (needs pull-up) |
| Pin 3 (NC) | Not connected | No connection |
| Pin 4 (GND) | GND | Ground |

**Note:** Many DHT11 modules come with a built-in pull-up resistor. Check your module before adding an external resistor.

## ⚙️ Configuration

### STM32CubeMX Setup

#### 1. Configure GPIO Pin

1. Open STM32CubeMX and create/open your project
2. Select a GPIO pin (e.g., PA0) for DHT11 data line
3. Set the pin as **GPIO_Output** initially
4. Configure GPIO settings:
   - Mode: Output Open Drain (or Output Push Pull)
   - Pull-up/Pull-down: Pull Up (if no external resistor)
   - Maximum output speed: High

#### 2. Configure Timer

1. Enable a timer (TIM1, TIM2, TIM3, or TIM4)
2. Set the timer configuration:
   - **Clock Source**: Internal Clock
   - **Prescaler**: Calculate for 1 MHz (1μs resolution)
     - Example: For 72 MHz system clock: Prescaler = 72-1 = 71
     - Example: For 84 MHz system clock: Prescaler = 84-1 = 83
     - Example: For 48 MHz system clock: Prescaler = 48-1 = 47
   - **Counter Period**: 0xFFFF (maximum)
   - **Auto-reload preload**: Enabled

#### 3. Generate Code

1. Click "Generate Code" in STM32CubeMX
2. Open the project in your IDE

### Prescaler Calculation

To achieve 1 μs timer resolution:

```
Prescaler = (SystemCoreClock / 1000000) - 1
```

| System Clock | Prescaler Value |
|--------------|-----------------|
| 48 MHz | 47 |
| 72 MHz | 71 |
| 84 MHz | 83 |
| 100 MHz | 99 |
| 168 MHz | 167 |

## 🚀 Usage

### Basic Example

```c
#include "main.h"
#include "dht11.h"

DHT11_Dev dht11;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    
    // Initialize DHT11
    DHT11_Init(&dht11, TIM2, GPIOA, GPIO_PIN_0);
    
    while (1)
    {
        // Read sensor data
        DHT11_Status status = DHT11_Read(&dht11);
        
        if (status == DHT11_OK)
        {
            printf("Temperature: %.1f°C\n", dht11.temperature);
            printf("Humidity: %.1f%%\n", dht11.humidity);
        }
        else
        {
            printf("Error reading DHT11: %d\n", status);
        }
        
        HAL_Delay(2000); // Wait 2 seconds between readings
    }
}
```

### Advanced Example with Error Handling

```c
#include "main.h"
#include "dht11.h"
#include <stdio.h>

DHT11_Dev dht11;
extern UART_HandleTypeDef huart2; // For printf

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_TIM2_Init();
    
    // Initialize DHT11
    DHT11_Init(&dht11, TIM2, GPIOA, GPIO_PIN_0);
    
    printf("DHT11 Sensor initialized\n");
    
    uint32_t readCount = 0;
    uint32_t errorCount = 0;
    
    while (1)
    {
        DHT11_Status status = DHT11_Read(&dht11);
        readCount++;
        
        switch (status)
        {
            case DHT11_OK:
                printf("[%lu] Temp: %.1f°C, Humidity: %.1f%%, Success Rate: %.1f%%\n",
                       readCount,
                       dht11.temperature,
                       dht11.humidity,
                       ((readCount - errorCount) * 100.0f) / readCount);
                break;
                
            case DHT11_ERROR_CHECKSUM:
                printf("[%lu] Checksum error!\n", readCount);
                errorCount++;
                break;
                
            case DHT11_ERROR_TIMEOUT:
                printf("[%lu] Timeout error!\n", readCount);
                errorCount++;
                break;
                
            case DHT11_ERROR_NO_RESPONSE:
                printf("[%lu] No response from sensor!\n", readCount);
                errorCount++;
                break;
                
            default:
                printf("[%lu] Unknown error: %d\n", readCount, status);
                errorCount++;
                break;
        }
        
        HAL_Delay(2000); // DHT11 requires minimum 1 second between readings
    }
}
```

### Temperature Conversion Example

```c
// Read in Celsius (default)
if (DHT11_Read(&dht11) == DHT11_OK)
{
    float tempC = dht11.temperature;
    float tempF = DHT11_ConvertToFahrenheit(tempC);
    
    printf("Temperature: %.1f°C / %.1f°F\n", tempC, tempF);
    printf("Humidity: %.1f%%\n", dht11.humidity);
}
```

## 📚 API Reference

### Initialization

#### `DHT11_Init()`

```c
void DHT11_Init(DHT11_Dev* dev, TIM_HandleTypeDef* timer, 
                GPIO_TypeDef* gpio_port, uint16_t gpio_pin);
```

Initializes the DHT11 sensor.

**Parameters:**
- `dev` - Pointer to DHT11_Dev structure
- `timer` - Pointer to timer handle (TIM1-TIM4)
- `gpio_port` - GPIO port (GPIOA, GPIOB, etc.)
- `gpio_pin` - GPIO pin number (GPIO_PIN_0, GPIO_PIN_1, etc.)

**Example:**
```c
DHT11_Dev dht11;
DHT11_Init(&dht11, TIM2, GPIOA, GPIO_PIN_0);
```

### Reading Data

#### `DHT11_Read()`

```c
DHT11_Status DHT11_Read(DHT11_Dev* dev);
```

Reads temperature and humidity from the DHT11 sensor.

**Parameters:**
- `dev` - Pointer to DHT11_Dev structure

**Returns:**
- `DHT11_OK` - Read successful
- `DHT11_ERROR_CHECKSUM` - Checksum verification failed
- `DHT11_ERROR_TIMEOUT` - Communication timeout
- `DHT11_ERROR_NO_RESPONSE` - Sensor did not respond

**Example:**
```c
if (DHT11_Read(&dht11) == DHT11_OK) {
    printf("Temp: %.1f°C\n", dht11.temperature);
    printf("Humidity: %.1f%%\n", dht11.humidity);
}
```

### Utility Functions

#### `DHT11_ConvertToFahrenheit()`

```c
float DHT11_ConvertToFahrenheit(float celsius);
```

Converts temperature from Celsius to Fahrenheit.

**Parameters:**
- `celsius` - Temperature in Celsius

**Returns:**
- Temperature in Fahrenheit

**Example:**
```c
float tempF = DHT11_ConvertToFahrenheit(25.0f); // Returns 77.0
```

#### `DHT11_GetErrorMsg()`

```c
const char* DHT11_GetErrorMsg(DHT11_Status status);
```

Returns error message string for a given status code.

**Parameters:**
- `status` - DHT11_Status code

**Returns:**
- Pointer to error message string

**Example:**
```c
DHT11_Status status = DHT11_Read(&dht11);
printf("Status: %s\n", DHT11_GetErrorMsg(status));
```

### Data Structures

#### `DHT11_Dev`

```c
typedef struct {
    float temperature;           // Temperature in Celsius
    float humidity;             // Relative humidity in %
    TIM_HandleTypeDef* timer;   // Timer handle
    GPIO_TypeDef* gpio_port;    // GPIO port
    uint16_t gpio_pin;          // GPIO pin
} DHT11_Dev;
```

#### `DHT11_Status`

```c
typedef enum {
    DHT11_OK = 0,              // Success
    DHT11_ERROR_CHECKSUM,      // Checksum error
    DHT11_ERROR_TIMEOUT,       // Timeout error
    DHT11_ERROR_NO_RESPONSE    // No response from sensor
} DHT11_Status;
```

## 💡 Examples

The `examples/` folder contains complete working examples:

### 1. Basic Reading (`examples/01_basic/`)
Simple temperature and humidity reading with UART output.

### 2. LCD Display (`examples/02_lcd_display/`)
Display sensor data on 16x2 LCD using I2C.

### 3. OLED Display (`examples/03_oled_display/`)
Show temperature and humidity on SSD1306 OLED display.

### 4. Data Logger (`examples/04_data_logger/`)
Log sensor data to SD card with timestamps.

### 5. Threshold Alert (`examples/05_threshold_alert/`)
Trigger LED/buzzer when temperature or humidity exceeds thresholds.

### 6. Moving Average Filter (`examples/06_moving_average/`)
Apply moving average filter to smooth sensor readings.

## 🐛 Troubleshooting

### Problem: No response from sensor

**Possible causes:**
- Incorrect wiring
- Missing or wrong value pull-up resistor
- Insufficient power supply
- Damaged sensor

**Solutions:**
- Check all connections carefully
- Use 4.7kΩ - 10kΩ pull-up resistor on data line
- Ensure stable 3.3V or 5V power supply
- Try a different DHT11 sensor

### Problem: Checksum errors

**Possible causes:**
- Timing issues
- Electromagnetic interference
- Long wires or poor connections
- Unstable power supply

**Solutions:**
- Verify timer prescaler is correct for 1μs resolution
- Use short wires (< 20 cm recommended)
- Add 100nF bypass capacitor near sensor VCC pin
- Check power supply stability

### Problem: Incorrect readings

**Possible causes:**
- Reading sensor too frequently
- Sensor requires warm-up time
- Calibration needed

**Solutions:**
- Wait minimum 2 seconds between readings
- Allow 5-10 seconds after power-on before first reading
- DHT11 is not highly accurate; consider DHT22 for better accuracy

### Problem: Timer not working

**Possible causes:**
- Timer not enabled in STM32CubeMX
- Incorrect prescaler value
- Timer clock not configured

**Solutions:**
- Enable timer in STM32CubeMX configuration
- Calculate correct prescaler: `(SystemClock / 1000000) - 1`
- Check timer clock source is enabled

### Problem: Compilation errors

**Possible causes:**
- Missing HAL drivers
- Incorrect include paths
- Wrong STM32 series configuration

**Solutions:**
- Ensure HAL drivers are generated in STM32CubeMX
- Add `Inc` folder to include paths
- Verify correct MCU is selected in project settings

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/new-feature`
3. Commit your changes: `git commit -m 'Add new feature'`
4. Push to the branch: `git push origin feature/new-feature`
5. Submit a pull request

### Contribution Guidelines

- Follow existing code style and formatting
- Add comments for complex logic
- Update documentation for new features
- Test on multiple STM32 boards if possible
- Include example code for new features

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 Kingkiri0986

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...
```

## 🙏 Acknowledgments

- STMicroelectronics for STM32 HAL libraries
- DHT11 sensor manufacturer (Aosong Electronics)
- STM32 community for support and inspiration
- Contributors and testers

## 📞 Contact

- **Author**: Kingkiri0986
- **Email**: paramtap0809@gmail.com
- **GitHub**: [@Kingkiri0986](https://github.com/Kingkiri0986)
- **Issues**: [GitHub Issues](https://github.com/Kingkiri0986/dht11_stm32/issues)

## 🔗 Related Projects

- [DHT22_STM32](https://github.com/Kingkiri0986/dht22_stm32) - Library for DHT22/AM2302 sensor
- [BME280_STM32](https://github.com/Kingkiri0986/bme280_stm32) - BME280 sensor library
- [STM32_Sensors](https://github.com/Kingkiri0986/stm32_sensors) - Collection of sensor libraries

## ⭐ Show Your Support

If you find this project helpful, please consider giving it a ⭐ on GitHub!

---

**Made with ❤️ for the STM32 community**