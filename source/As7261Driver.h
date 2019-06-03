/*
 * Copyright (c) 2019 Future Electronics
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AS7261_DRIVER_H_
#define AS7261_DRIVER_H_

#include <stdint.h>
#include "TypedEnum.h"
#include <Sensor.h>


/** Driver for AS7261 light sensor.
 */
class As7261Driver {
public:
    enum Status {
        STATUS_OK = 0,
        STATUS_STALLED,
        STATUS_NOT_READY
    };

public:
    /** Create and initialize driver.
     *
     * @param bus I2C bus to use for communication
     * @param address I2C address to use
     */
    As7261Driver(I2C& bus, uint8_t address) : _i2c(bus), _address(address), _last_i(0), _last_t(0) {};

    /** Read measured values from sensor.
     */
    Status read(uint32_t &lux, uint32_t &cct, uint8_t &red, uint8_t &green, uint8_t &blue );

    /** Set lighting led status.
     */
    Status led_on(bool state);

    /** Initialize chip.
     */
    void init_chip();

    /** Start measurement cycle.
     */
    void start_measurement();

protected:
    enum PhyReg {
        STATUS_REG  = 0x00,
        WRITE_REG,
        READ_REG
    };

    enum VirtualReg {
        SETUP_CONTROL   = 0x04,
        INT_TIME        = 0x05,
        LED_CONTROL     = 0x07,
        COLOR_X         = 0x14,
        COLOR_Y         = 0x18,
        COLOR_Z         = 0x1C,
        CAL_LUX         = 0x3C,
        CAL_CCT         = 0x3E,
        WRITE_OP        = 0x80
    };

    static const uint32_t   LUXCCT_REG_SIZE = 2;
    static const uint32_t   COLOR_REG_SIZE = 4;

    class StatusReg : public TypedEnum<uint8_t> {
    public:
        enum enum_t {
            RX_PENDING = 0x01,
            TX_PENDING = 0x02
        };

        StatusReg(enum_t init_val) : TypedEnum<uint8_t>(init_val) {}
    };

    class ControlReg : public TypedEnum<uint8_t> {
    public:
        enum enum_t {
            RESET      = 0x80,
            INT_EN     = 0x40,
            GAINx1     = 0x00,
            GAINx4     = 0x10,
            GAINx16    = 0x20,
            GAINx64    = 0x30,
            MODE_0     = 0x00,
            MODE_1     = 0x04,
            MODE_2     = 0x08,
            MODE_3     = 0x0C,
            DATA_RDY   = 0x02
        };

        ControlReg(enum_t init_val) : TypedEnum<uint8_t>(init_val) {}
    };

    class LedControlReg : public TypedEnum<uint8_t> {
    public:
        enum enum_t {
            LED_DRV_12mA   = 0x08,
            LED_DRV_25mA   = 0x18,
            LED_DRV_50mA   = 0x28,
            LED_DRV_100mA  = 0x38,
            LED_INT_1mA    = 0x01,
            LED_INT_2mA    = 0x03,
            LED_INT_4mA    = 0x05,
            LED_INT_8mA    = 0x07,
            LED_OFF        = 0x00
        };

        LedControlReg(enum_t init_val) : TypedEnum<uint8_t>(init_val) {}
    };

protected:
    int read_phy_reg(PhyReg reg, uint8_t &value);
    int write_phy_reg(PhyReg reg, uint8_t value);
    int check_buffer_status(uint8_t mask, uint8_t required);
    Status read_register(VirtualReg reg, uint8_t &value);
    Status write_register(VirtualReg reg, uint8_t value);
    Status read_value(VirtualReg reg, uint32_t len, uint32_t &value);

protected:
    I2C&        _i2c;
    uint8_t     _address;
    uint16_t    _last_i;
    uint16_t    _last_t;
};


#endif // AS7261_DRIVER_H_
