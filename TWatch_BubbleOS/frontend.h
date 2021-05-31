////////////////////--Tasks--////////////////////////////////////
bool FEND_home(){
 APP_digitalClock();
 APP_drawBattery();
 return false;
}



///////////////////--Main Thread--//////////////////////////////
void FEND_begin(int x, int y)
{

 if(!FEND_task[0])
 FEND_task[0] = FEND_home();

}
