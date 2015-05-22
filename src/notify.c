#include "notify.h"
#include <pebble.h>
#include "clock.h"
  
static int count_down=15;
static AppTimer *timer_handle;
static bool is_done=false;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_gothic_24_bold;
static TextLayer *count_down_tl;
static TextLayer *cancel_mes_tl;
static TextLayer *sending_tl;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, true);
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // count_down_tl
  count_down_tl = text_layer_create(GRect(43, 37, 56, 54));
  text_layer_set_background_color(count_down_tl, GColorBlack);
  text_layer_set_text_color(count_down_tl, GColorWhite);
  text_layer_set_text(count_down_tl, "15");
  text_layer_set_text_alignment(count_down_tl, GTextAlignmentCenter);
  text_layer_set_font(count_down_tl, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)count_down_tl);
  
  // cancel_mes_tl
  cancel_mes_tl = text_layer_create(GRect(0, 96, 144, 50));
  text_layer_set_text(cancel_mes_tl, "Press any button to cancel");
  text_layer_set_text_alignment(cancel_mes_tl, GTextAlignmentCenter);
  text_layer_set_font(cancel_mes_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)cancel_mes_tl);
  
  // sending_tl
  sending_tl = text_layer_create(GRect(23, 3, 100, 30));
  text_layer_set_text(sending_tl, "Sending alert");
  text_layer_set_font(sending_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)sending_tl);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(count_down_tl);
  text_layer_destroy(cancel_mes_tl);
  text_layer_destroy(sending_tl);
}
// END AUTO-GENERATED UI CODE

static void out_sent_handler(DictionaryIterator *sent, void *context) {
   // outgoing message was delivered
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
}

static void in_received_handler(DictionaryIterator *received, void *context) {

}

static void in_dropped_handler(AppMessageResult reason, void *context) {
   // incoming message dropped
}

void app_message_init(void){
  app_message_register_outbox_sent(out_sent_handler);
  app_message_register_outbox_failed(out_failed_handler);
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);
}

static void send_int(int key, int value) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &value, sizeof(int), true);
  app_message_outbox_send();
}

void count_down_timer_callback(void *data){
  static char buf[] = "12";
  snprintf(buf, sizeof(buf), "%d", --count_down);
  text_layer_set_text(count_down_tl, buf);
  if(count_down<5&&count_down>0){
    vibes_short_pulse();
  }
  if(count_down==0){
    send_int(1,42);
    vibes_long_pulse();
    is_done=true;
    window_stack_pop(true);
  }else{
    timer_handle=app_timer_register(1000,count_down_timer_callback,NULL);
  }
}

void notify_single_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_pop(true);
}

void notify_config_provider(Window *window) {
 // single click 
  window_single_click_subscribe(BUTTON_ID_BACK, notify_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, notify_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN,notify_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT,notify_single_click_handler);
}



static void handle_window_unload(Window* window) {
  if(is_done==false){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "canceling timer");
    app_timer_cancel(timer_handle);
  }
  reset_is_launched();
  app_worker_launch();
  destroy_ui();
}

void show_notify(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_set_click_config_provider(s_window, (ClickConfigProvider) notify_config_provider);
  app_message_init();
  timer_handle=app_timer_register(1000,count_down_timer_callback,NULL);
  count_down=15;
  is_done=false;
  app_worker_kill();
  vibes_long_pulse();
  window_stack_push(s_window, true);
}

void hide_notify(void) {
  window_stack_remove(s_window, true);
}
