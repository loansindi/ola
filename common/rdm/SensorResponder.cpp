/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * SensorResponder.cpp
 * Copyright (C) 2013 Simon Newton
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include "ola/BaseTypes.h"
#include "ola/Clock.h"
#include "ola/Logging.h"
#include "ola/base/Array.h"
#include "ola/stl/STLUtils.h"
#include "ola/math/Random.h"
#include "ola/network/NetworkUtils.h"
#include "ola/rdm/OpenLightingEnums.h"
#include "ola/rdm/RDMEnums.h"
#include "ola/rdm/ResponderHelper.h"
#include "ola/rdm/ResponderSensor.h"
#include "ola/rdm/SensorResponder.h"

namespace ola {
namespace rdm {

using ola::network::HostToNetwork;
using ola::network::NetworkToHost;
using std::string;
using std::vector;

SensorResponder::RDMOps *SensorResponder::RDMOps::instance = NULL;

const ResponderOps<SensorResponder>::ParamHandler
    SensorResponder::PARAM_HANDLERS[] = {
  { PID_DEVICE_INFO,
    &SensorResponder::GetDeviceInfo,
    NULL},
  { PID_PRODUCT_DETAIL_ID_LIST,
    &SensorResponder::GetProductDetailList,
    NULL},
  { PID_DEVICE_MODEL_DESCRIPTION,
    &SensorResponder::GetDeviceModelDescription,
    NULL},
  { PID_MANUFACTURER_LABEL,
    &SensorResponder::GetManufacturerLabel,
    NULL},
  { PID_DEVICE_LABEL,
    &SensorResponder::GetDeviceLabel,
    NULL},
  { PID_SOFTWARE_VERSION_LABEL,
    &SensorResponder::GetSoftwareVersionLabel,
    NULL},
  { PID_SENSOR_DEFINITION,
    &SensorResponder::GetSensorDefinition,
    NULL},
  { PID_SENSOR_VALUE,
    &SensorResponder::GetSensorValue,
    &SensorResponder::SetSensorValue},
  { PID_RECORD_SENSORS,
    NULL,
    &SensorResponder::RecordSensor},
  { PID_IDENTIFY_DEVICE,
    &SensorResponder::GetIdentify,
    &SensorResponder::SetIdentify},
  { 0, NULL, NULL},
};


/**
 * A class which represents a sensor.
 */
class FakeSensor: public Sensor {
  public:
    FakeSensor(ola::rdm::rdm_sensor_type type,
               ola::rdm::rdm_pid_unit unit,
               ola::rdm::rdm_pid_prefix prefix,
               const string &description = "",
               bool recorded_value_support = true,
               bool recorded_range_support = true,
               int16_t range_min = SENSOR_DEFINITION_RANGE_MIN_UNDEFINED,
               int16_t range_max = SENSOR_DEFINITION_RANGE_MAX_UNDEFINED,
               int16_t normal_min = SENSOR_DEFINITION_NORMAL_MIN_UNDEFINED,
               int16_t normal_max = SENSOR_DEFINITION_NORMAL_MAX_UNDEFINED)
        : Sensor(type,
                 unit,
                 prefix,
                 description,
                 recorded_value_support,
                 recorded_range_support,
                 range_min,
                 range_max,
                 normal_min,
                 normal_max) {
      // set high / low to something
      Reset();
      // Force recorded back to zero
      m_recorded = 0;
    }

    int16_t PollSensor();
    int16_t GenerateValue();
};


/**
 * Generate a Sensor value
 */
int16_t FakeSensor::GenerateValue() {
  int16_t value = ola::math::Random(m_range_min, m_range_max);
  return value;
}


/**
 * Fetch a Sensor value
 */
int16_t FakeSensor::PollSensor() {
  // This is a fake sensor, so make a value
  return GenerateValue();
}


/**
 * New SensorResponder
 */
SensorResponder::SensorResponder(const UID &uid)
    : m_uid(uid),
      m_identify_mode(false) {
  m_sensors.push_back(new FakeSensor(
        SENSOR_TEMPERATURE, UNITS_CENTIGRADE, PREFIX_NONE,
        "Fake Temperature", true, true, 0, 100, 10, 20));
  m_sensors.push_back(new FakeSensor(
        SENSOR_VOLTAGE, UNITS_VOLTS_DC, PREFIX_DECI,
        "Fake Voltage", true, true, 110, 140, 119, 125));
  m_sensors.push_back(new FakeSensor(
        SENSOR_ITEMS, UNITS_NONE, PREFIX_KILO,
        "Fake Beta Particle Counter", true, true, 0, 100, 0, 1));
}


SensorResponder::~SensorResponder() {
  STLDeleteElements(&m_sensors);
}

/*
 * Handle an RDM Request
 */
void SensorResponder::SendRDMRequest(const RDMRequest *request,
                                          RDMCallback *callback) {
  RDMOps::Instance()->HandleRDMRequest(this, m_uid, ROOT_RDM_DEVICE, request,
                                       callback);
}

const RDMResponse *SensorResponder::GetDeviceInfo(
    const RDMRequest *request) {
  return ResponderHelper::GetDeviceInfo(
      request, OLA_SENSOR_ONLY_MODEL, PRODUCT_CATEGORY_TEST,
      1, 0, 1, 1, ZERO_FOOTPRINT_DMX_ADDRESS, 0, m_sensors.size());
}

const RDMResponse *SensorResponder::GetProductDetailList(
    const RDMRequest *request) {
  // Shortcut for only one item in the vector
  return ResponderHelper::GetProductDetailList(
      request, vector<rdm_product_detail>(1, PRODUCT_DETAIL_TEST));
}

const RDMResponse *SensorResponder::GetIdentify(
    const RDMRequest *request) {
  return ResponderHelper::GetBoolValue(request, m_identify_mode);
}

const RDMResponse *SensorResponder::SetIdentify(
    const RDMRequest *request) {
  bool old_value = m_identify_mode;
  const RDMResponse *response = ResponderHelper::SetBoolValue(
      request, &m_identify_mode);
  if (m_identify_mode != old_value) {
    OLA_INFO << "Sensor Device " << m_uid << ", identify mode "
             << (m_identify_mode ? "on" : "off");
  }
  return response;
}

const RDMResponse *SensorResponder::GetDeviceModelDescription(
    const RDMRequest *request) {
  return ResponderHelper::GetString(request, "OLA Sensor Device");
}

const RDMResponse *SensorResponder::GetManufacturerLabel(
    const RDMRequest *request) {
  return ResponderHelper::GetString(request, OLA_MANUFACTURER_LABEL);
}

const RDMResponse *SensorResponder::GetDeviceLabel(
    const RDMRequest *request) {
  return ResponderHelper::GetString(request, "Sensor Device");
}

const RDMResponse *SensorResponder::GetSoftwareVersionLabel(
    const RDMRequest *request) {
  return ResponderHelper::GetString(request, string("OLA Version ") + VERSION);
}

/**
 * PID_SENSOR_DEFINITION
 */
const RDMResponse *SensorResponder::GetSensorDefinition(
    const RDMRequest *request) {
  return ResponderHelper::GetSensorDefinition(request, &m_sensors);
}

/**
 * PID_SENSOR_VALUE
 */
const RDMResponse *SensorResponder::GetSensorValue(const RDMRequest *request) {
  return ResponderHelper::GetSensorValue(request, &m_sensors);
}

const RDMResponse *SensorResponder::SetSensorValue(const RDMRequest *request) {
  return ResponderHelper::SetSensorValue(request, &m_sensors);
}

/**
 * PID_RECORD_SENSORS
 */
const RDMResponse *SensorResponder::RecordSensor(const RDMRequest *request) {
  return ResponderHelper::RecordSensor(request, &m_sensors);
}
}  // namespace rdm
}  // namespace ola
