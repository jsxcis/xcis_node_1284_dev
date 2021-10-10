bool onReceive()
{
  uint8_t buf[32] = {0};
  uint8_t len = sizeof(buf);
  uint8_t from;
  String response = "";
  uint8_t recvPayload[28];
   
  if (manager->recvfromAck(buf, &len, &from))
  {
    // Assume message is for me.
    digitalWrite(LORA,1); 
    Serial.print("onReceive()");
    //Serial.println((char*)buf);
    xcisMessage.dumpHex(buf,sizeof(buf));
    xcisMessage.processMessage(buf);
    Serial.print(" LocationID:");
    Serial.print(xcisMessage.getLocationID(),HEX);
    Serial.print(" DeviceType:");
    Serial.print(xcisMessage.getDeviceType(),HEX);
    Serial.print(" Command:");
    Serial.print(xcisMessage.getCommand(),HEX);
    xcisMessage.getPayload(recvPayload);
    Serial.print(" Payload:");
    xcisMessage.dumpHex(recvPayload,28);

    if (xcisMessage.getCommand() == SENSOR_DATA_REQUEST)
    { 
      Serial.println("Received:SENSOR_DATA_REQUEST");
      Serial.print("Voltage value:");
      Serial.println(getVoltageBytes());
      uint16_t battery = getVoltageBytes();
      Serial.println(battery,HEX);
     
      Serial.print("Pulses Integer:");
      Serial.println(accumulatedPulses);
     Serial.print("Pulses Hex:");
      Serial.println(accumulatedPulses,HEX);
      
       Serial.print("accumulatedDataToken Integer:");
      Serial.println(accumulatedDataToken);
     Serial.print("accumulatedDataToken Hex:");
      Serial.println(accumulatedDataToken,HEX);
      
      xcisMessage.createPulseCounterPayload(SENSOR_DATA_RESPONSE, battery,accumulatedPulses, accumulatedDataToken);  
      xcisMessage.createMessage(data,xcisMessage.getLocationID(), DEVICE_TYPE, SENSOR_DATA_RESPONSE);
  
      Serial.print("Response:");
      xcisMessage.dumpHex(data,32);

      manager->sendtoWait(data, sizeof(data), from);
    
      
      //response = "ID=" + node_id_default + ",B=" + getVoltage().substring(0,3) + ",V=" + String(accumulatedPulses) + ",T=" + String(accumulatedDataToken)  + ",";
        //response = "ID=" + getChipID_string() + ",B=" + getVoltage().substring(0,3) + ",V=" + String(accumulatedPulses) + ",T=" + String(accumulatedDataToken)  + ",";
      //Serial.println(response); 
      //sendMessage(response, from);
    }
    digitalWrite(LORA,0);
  }
  return true;
}
