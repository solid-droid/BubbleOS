void SYS_devicePower(bool value)
{
  for(int i=0; i< sizeof(SYS_devices); ++i)
  {
    BOOT_power(SYS_devices[i], value);
  }
}
void SYS_sleep()
{
 BOOT_power(SYS_devices[2], false);
}

void SYS_wakeup()
{
  BOOT_power(SYS_devices[2], true);
}

void SYS_powerOFF()
{
   SYS_sleep();
   BOOT_deepSleep();
}
