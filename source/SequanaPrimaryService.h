/*
 * Copyright (c) 2017-2018 Future Electronics
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

#ifndef SEQUANAPRIMARYSERVICE_H__
#define SEQUANAPRIMARYSERVICE_H__

#include "ble/BLE.h"
#include "UUID.h"
#include "Sensor.h"
#include "SensorCharacteristic.h"
#include "Kx64.h"
#include "Sps30.h"
#include "ComboEnvSensor.h"
#include "AirQSensor.h"

namespace sequana {

/* Instantiation of binary buffers for characteristics values */
typedef CharBuffer<Kx64Value, 12>   Kx64CharBuffer;

typedef CharBuffer<Sps30Value, 12>  Sps30CharBuffer;

//typedef CharBuffer<AirQValue, 12>   AirQCharBuffer;

#if 1
class AirQCharBuffer : public CharBuffer<AirQValue, 10> {
public:
    AirQCharBuffer& operator= (const AirQValue &val)
    {
        memcpy(_bytes, &val.tvoc, 2);
        memcpy(_bytes+2, &val.eco2, 4);
        memcpy(_bytes+6, &val.co2, 4);
        return *this;
    }
};
#endif

class ComboEnvCharBuffer : public CharBuffer<ComboEnvValue, 11> {
public:
    ComboEnvCharBuffer& operator= (const ComboEnvValue &val)
    {
        memcpy(_bytes, &val.temperature, 2);
        memcpy(_bytes+2, &val.noise, 2);
        memcpy(_bytes+4, &val.ambient_light, 3);
        memcpy(_bytes+7, &val.humidity, 2);
        memcpy(_bytes+9, &val.color_temp, 2);
        return *this;
    }
};



/** Sequana BLE Primary Service. This service provides data obtained from Sequana and Sequana Environmental Shield sensors.
 */
class PrimaryService {
public:
    static const UUID UUID_SEQUANA_PRIMARY_SERVICE;

public:
    /** Add the Sequana Primary Service to an existing BLE object, initializing it with all characteristics.
     * @param ble                   Reference to the BLE device.
     * @param accmag_sensor         Reference to KX64 sensor.
     * @param partmatter_sensor     Reference to PSP30 sensor.
     * @param combo_env_sensor      Reference to combined parameters sensor.
     */
    PrimaryService(BLE &ble,
#ifdef TARGET_FUTURE_SEQUANA
                   Kx64Sensor &accmag_sensor,
#endif //TARGET_FUTURE_SEQUANA
                   Sps30Sensor &partmatter_sensor,
                   ComboEnvSensor &combo_env_sensor,
                   AirQSensor &airq_sensor );

protected:
    BLE &_ble;
#ifdef TARGET_FUTURE_SEQUANA
    SensorMultiCharacteristic<2, Kx64CharBuffer, Kx64Value>         _accMagSensorMeasurement;
#endif //TARGET_FUTURE_SEQUANA
    SensorCharacteristic<Sps30CharBuffer, Sps30Value>               _particulateMatterMeasurement;
    SensorMultiCharacteristic<2, ComboEnvCharBuffer, ComboEnvValue> _comboEnvMeasurement;
    SensorCharacteristic<AirQCharBuffer, AirQValue>                 _airQMeasurement;


};

} //namespace

#endif // SEQUANAPRIMARYSERVICE_H__
