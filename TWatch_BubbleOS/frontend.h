void FEND_home(void *parameters){
  for(;;){
     delay(1);
     APP_digitalClock();
     APP_drawText(String(battery)+'%', 3, 3, 4); 
  }
}
