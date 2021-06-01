void FEND_home(void *parameters){
  for(;;){
     APP_digitalClock();
     APP_drawText(String(battery)+'%', 3, 3, 4); 
  }
}
