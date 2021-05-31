////////////////////--Frontend Variables--///////////////////////
lv_obj_t *btn1;

lv_obj_t *label2;
lv_obj_t *btn2;
bool bt2_state = false;
////////////////////--Event-Handlers--///////////////////////////
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {

    } else if (event == LV_EVENT_VALUE_CHANGED) {
      bt2_state = !bt2_state;
      lv_label_set_text(label2 , bt2_state?"ON":"OFF" );
    }
}
////////////////////--Tasks--////////////////////////////////////
bool FEND_home(){
    lv_obj_t *label;
    btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn1, event_handler);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Button");

    btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn2, event_handler);
    lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 0, 40);
    lv_btn_set_checkable(btn2, true);
    lv_btn_toggle(btn2);
    lv_btn_set_fit2(btn2, LV_FIT_NONE, LV_FIT_TIGHT);

    label2 = lv_label_create(btn2, NULL);
    lv_label_set_text(label2, "false");
 return true;
}

void FEND_begin(int x, int y)
{
// ////////--Battery Status--////////
// ttgo->tft->drawString(String(battery), 10, 50); 
 ///////---Task 0 ---//////////
 if(!FEND_task[0])
 FEND_task[0] = FEND_home();

}
