void BEND_begin(int x, int y)
{
  /////////--Brightness--////////////
  if(drag && y < 30){
    BOOT_setBrightness(map(x, 3, 230, 10, 240));
  }
}
