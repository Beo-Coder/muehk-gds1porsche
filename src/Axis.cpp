//
// Created by Leo on 05.04.2023.
//
#include "Arduino.h"
#include "EEPROM.h"
#include "Axis.h"

#define DEBOUNCE_TIME 50


void waitUntilButtonPressed(uint8_t pin) {
    while (digitalRead(pin) == 0);
    unsigned long previousMillis = millis();
    while (millis() - previousMillis < DEBOUNCE_TIME);
    while (digitalRead(pin) == 1);
    while (millis() - previousMillis < DEBOUNCE_TIME);
}


Axis::Axis(uint8_t pin, const String &name, bool linear, bool calibrateCenter,
           uint8_t eepromAddressCalibrationDataBegin) {
    this->pin = pin;
    this->name = name;
    this->linear = linear;
    this->calibrateCenter = calibrateCenter;
    this->eepromAddressCalibrationDataBegin = eepromAddressCalibrationDataBegin;

    currentRawValue = 0;
    previousRawValue = 1;
}

uint16_t Axis::readSensor() const {
    return analogRead(pin);
}

double Axis::logCalculation(double value) {
    return pow(base[1], double(value) - calibrationData[2]) -
           pow(base[1], -double(value) + calibrationData[1] - calibrationData[2]);
}

void Axis::calculateBase(){
    base[0] = pow(1028, 1 / double(calibrationData[1] - calibrationData[2]));
    base[1] = pow(1028, 1 / double(calibrationData[2]));
}

void Axis::calibrateNew(uint8_t enterButtonPin, bool writeToEEPROM) {
    Serial.println("Calibration for " + name + " started!");
    Serial.println("Please set " + name + " to minimum position and press calibration button.");
    waitUntilButtonPressed(enterButtonPin);
    calibrationData[0] = readSensor();


    Serial.println("Please set " + name + " to maximum position and press calibration button.");
    waitUntilButtonPressed(enterButtonPin);
    calibrationData[1] = readSensor();


    if (calibrateCenter) {
        Serial.println("Please set " + name + " to center position and press calibration button.");
        waitUntilButtonPressed(enterButtonPin);
        calibrationData[2] = readSensor();
    } else {
        if (calibrationData[1] >= calibrationData[0]) {
            calibrationData[2] = (calibrationData[1] - calibrationData[0]) / 2;
        } else {
            calibrationData[2] = (calibrationData[0] - calibrationData[1]) / 2;
        }

    }
    if(writeToEEPROM){
        EEPROM.write(eepromAddressCalibrationDataBegin, (calibrationData[0] >> 8) & 0xFF);
        EEPROM.write(eepromAddressCalibrationDataBegin+1, calibrationData[0] & 0xFF);

        EEPROM.write(eepromAddressCalibrationDataBegin+2, (calibrationData[1] >> 8) & 0xFF);
        EEPROM.write(eepromAddressCalibrationDataBegin+3, (calibrationData[1]) & 0xFF);

        EEPROM.write(eepromAddressCalibrationDataBegin+4, (calibrationData[2] >> 8) & 0xFF);
        EEPROM.write(eepromAddressCalibrationDataBegin+5, (calibrationData[2]) & 0xFF);

    }



    calculateBase();


    Serial.println("Calibration for " + name + " done!");
}

void Axis::loadCalibration() {
    calibrationData[0] = EEPROM.read(eepromAddressCalibrationDataBegin) << 8 | EEPROM.read(eepromAddressCalibrationDataBegin+1);
    calibrationData[1] = EEPROM.read(eepromAddressCalibrationDataBegin+2) << 8 | EEPROM.read(eepromAddressCalibrationDataBegin+3);
    calibrationData[2] = EEPROM.read(eepromAddressCalibrationDataBegin+4) << 8 | EEPROM.read(eepromAddressCalibrationDataBegin+5);
    calculateBase();
}


void Axis::setBase(double pBase) {
    this->base[0] = pBase;
    this->base[1] = pBase;
}

void Axis::setBase(double pBase1, double pBase2) {
    base[0] = pBase1;
    base[1] = pBase2;
}

int32_t Axis::getValue() {
    int32_t value = 0;

    if (linear) {
        value = map(currentRawValue, calibrationData[0], calibrationData[1], -1028, 1028);
    } else {
        double calculationValue = logCalculation(double(currentRawValue));
        if (calculationValue >= 0) {
            value = int32_t(calculationValue + 0.5);
        } else {
            value = int32_t(calculationValue);
        }
        value = map(value, logCalculation(calibrationData[0]), logCalculation(calibrationData[1]), -1028, 1028);

    }

    return value;
}

bool Axis::valueChanged() {
    currentRawValue = readSensor();
    if (currentRawValue != previousRawValue) {
        previousRawValue = currentRawValue;
        return true;
    } else {
        return false;
    }
}

