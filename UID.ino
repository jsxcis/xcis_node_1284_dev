void initUID()
{
  pinMode(DATAOUT, OUTPUT);

  pinMode(DATAIN, INPUT);

  pinMode(SPICLOCK,OUTPUT);

  pinMode(SLAVESELECT,OUTPUT);

  //pinMode(HOLD,OUTPUT);

  //digitalWrite(HOLD,HIGH);
  
  digitalWrite(SLAVESELECT,HIGH); //disable device
  SPI.begin();
}
void read_device_code()
{
  unsigned int addr;
  byte one;
  byte two;
  byte three;
  byte four;
  Serial.println("Read Device Code");
  digitalWrite(SLAVESELECT, LOW);

  /* transmit read id command - jedec does not require 3 dummy bytes */
  SPI.transfer(READ);
  delay(10);
  
  SPI.transfer(0xFB);
  delay(10);
  one = SPI.transfer(0x00);
  Serial.println(one,HEX);
  digitalWrite(SLAVESELECT, HIGH);

  delay(10);
 
  digitalWrite(SLAVESELECT, HIGH);
}
void read_manufacturer() {
  unsigned int addr;
  byte one;
  byte two;
  byte three;
  byte four;
  Serial.println("Read Manufacturer");
  digitalWrite(SLAVESELECT, LOW);

  /* transmit read id command - jedec does not require 3 dummy bytes */
  SPI.transfer(READ);
  delay(10);
  
  SPI.transfer(0xFA);
  delay(10);
  one = SPI.transfer(0x00);
  Serial.println(one,HEX);
  digitalWrite(SLAVESELECT, HIGH);

  delay(10);
 
  digitalWrite(SLAVESELECT, HIGH);
}

void read_ID_new() {
  unsigned int addr;
  unsigned int one;
  unsigned int two;
  unsigned int three;
  unsigned int four;
  unsigned int five;
  unsigned int six;
  Serial.println("Read Serial number memory map");
 
  digitalWrite(SLAVESELECT, LOW);

  SPI.transfer(READ);
  delay(10);  
  SPI.transfer(0xFA);
  delay(10);
  one = SPI.transfer(0x00);
  Serial.println(one,HEX);
  two = SPI.transfer(0x00);
  Serial.println(two,HEX);
  three = SPI.transfer(0x00);
  Serial.println(three,HEX);
  four = SPI.transfer(0x00);
  Serial.println(four,HEX);
  five = SPI.transfer(0x00);
  Serial.println(five,HEX);
  six = SPI.transfer(0x00);
  Serial.println(six,HEX);

  digitalWrite(SLAVESELECT, HIGH);

}

void read_Status_Reg() {
  byte resp;
  digitalWrite(SLAVESELECT, LOW);
  SPI.transfer(RDSR); // returns one byte
  delay(10);

  resp = SPI.transfer(0x00);
  Serial.println("Status Register");
  Serial.print("bit 0 - WIP\t");
  Serial.println(bitRead(resp, 0));
  Serial.print("bit 1 - WEL\t");
  Serial.println(bitRead(resp, 1));
  Serial.print("bit 2 - BP0\t");
  Serial.println(bitRead(resp, 2));
  Serial.print("bit 3 - BP1\t");
  Serial.println(bitRead(resp, 3));
  Serial.print("bit 4 -\t");
  Serial.println(bitRead(resp, 4));
  Serial.print("bit 5 -\t");
  Serial.println(bitRead(resp, 5));
  Serial.print("bit 6 -\t");
  Serial.println(bitRead(resp, 6));
  Serial.print("bit 7 -\t");
  Serial.println(bitRead(resp, 7));
  digitalWrite(SLAVESELECT, HIGH);
}
