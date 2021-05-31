bool BEND_swipeBrightness(int x, int y)
{
  if(drag && y < 50){
    if(x<10)
    BOOT_setBrightness(7);
    else if (x < 200)
    BOOT_setBrightness(int(map(x, 10, 200, 10, 200)));
    else 
    BOOT_setBrightness(255);
  }

  return false;
}

void BEND_begin(int x, int y)
{
  if(!BEND_task[0])
    BEND_task[0] = BEND_swipeBrightness(x, y);
}
