bool checkLoraInitialised()
{
// read a byte from the current address of the EEPROM
  value = EEPROM.read(address);

  //Serial.print(address);
  //Serial.print("\t");
  //Serial.print(value, DEC);
  //Serial.println();

  if (value == 1)
  {
    return false;
  }
  else if (value == 2)
  {
    return true;
  }
}
void setLoraInitialised()
{
  EEPROM.write(address, 2);
}
void setLoraDefault()
{
  EEPROM.write(address, 1);
}
int getChipID()
{
  // Code to get the Chip ID
  int chipID = 2222;

  return chipID;
}
String getChipID_string()
{
  String chipID = "2222";
  // Code to get the Chip ID
  return chipID;
}
