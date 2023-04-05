
// Copyright (c) 2023. Leonhard Baschang


#ifndef UNTITLED19_AXIS_H
#define UNTITLED19_AXIS_H


class Axis {
    uint8_t pin;
    String name;
    uint16_t calibrationData[3]{};
    bool linear;
    bool calibrateCenter;
    double base[2]{};
    uint16_t currentRawValue;
    uint16_t previousRawValue;

    uint8_t eepromAddressCalibrationDataBegin;


    uint16_t readSensor() const;

    double logCalculation(double value);
    void calculateBase();

public:
    Axis(uint8_t pin, const String &name, bool linear, bool calibrateCenter, uint8_t eepromAddressCalibrationDataBegin);

    void calibrateNew(uint8_t enterButtonPins, bool writeToEEPROM);
    void loadCalibration();

    void setBase(double pBase);

    void setBase(double pBase1, double pBase2);

    int32_t getValue();

    bool valueChanged();


};


#endif //UNTITLED19_AXIS_H
