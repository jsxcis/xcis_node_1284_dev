String getVoltage()
{
    float batteryVoltage = 0; //
    batteryVoltage = (analogRead(BATTERY_VOLTAGE) * 0.006325);
    return String(batteryVoltage);
}

uint16_t getVoltageBytes()
{
    uint16_t batteryVoltage = 0; //
    batteryVoltage = (analogRead(BATTERY_VOLTAGE) * 0.006325) * 100;
    batteryVoltage = (512 * 0.006325) * 100;
    return batteryVoltage;
}
