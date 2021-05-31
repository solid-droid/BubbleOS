////////////////////--Tasks--////////////////////////////////////
bool FEND_home(){
 APP_digitalClock();
 APP_drawText(String(battery)+'%', 3, 3, 4); 
 return false;
}

bool FEND_batteryIcon()
{
  return true;
}

bool FEND_menuIcon()
{

  return true;
}

///////////////////--Main Thread--//////////////////////////////
void FEND_begin(int x, int y)
{

 if(!FEND_task[0])
 FEND_task[0] = FEND_home();

 if(!FEND_task[1])
 FEND_task[1] = FEND_batteryIcon();

 if(!FEND_task[2])
 FEND_task[2] = FEND_menuIcon();

}
