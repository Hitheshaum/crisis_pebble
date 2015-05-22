#include "settings.h"
#include <pebble.h>
static int threshold= 50;
static int up_pressed=0;
static int down_pressed=0;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_28_bold;
static GFont s_res_gothic_24_bold;
static TextLayer *treshold_tl;
static TextLayer *value_tl;
static TextLayer *ok_tl;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, true);
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // treshold_tl
  treshold_tl = text_layer_create(GRect(0, 0, 142, 19));
  text_layer_set_text(treshold_tl, "THRESHOLD");
  text_layer_set_text_alignment(treshold_tl, GTextAlignmentCenter);
  text_layer_set_font(treshold_tl, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)treshold_tl);
  
  // value_tl
  value_tl = text_layer_create(GRect(0, 69, 144, 36));
  text_layer_set_background_color(value_tl, GColorBlack);
  text_layer_set_text_color(value_tl, GColorWhite);
  text_layer_set_text(value_tl, "50");
  text_layer_set_text_alignment(value_tl, GTextAlignmentCenter);
  text_layer_set_font(value_tl, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)value_tl);
  
  // ok_tl
  ok_tl = text_layer_create(GRect(3, 23, 37, 28));
  text_layer_set_text(ok_tl, "< Ok");
  text_layer_set_font(ok_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)ok_tl);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(treshold_tl);
  text_layer_destroy(value_tl);
  text_layer_destroy(ok_tl);
}
// END AUTO-GENERATED UI CODE

static void update_threshold_value(void){
  static char buf[] = "100";
  snprintf(buf, sizeof(buf), "%d", threshold);
  text_layer_set_text(value_tl, buf);
}

void settings_up_single_click_handler(ClickRecognizerRef recognizer, void *context){
  if(threshold<100){
    threshold++;
    update_threshold_value();
  }  
}
void settings_down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  if(threshold>1){
    threshold--;
    update_threshold_value();
  }
}
void timer_callback(void *data){
  if(up_pressed==1&&threshold<100){
    threshold++;
    update_threshold_value();
    app_timer_register(100,timer_callback,NULL);
  }
  if(down_pressed==1&&threshold>1){
    threshold--;
    update_threshold_value();
    app_timer_register(100,timer_callback,NULL);
  }
}
void settings_up_long_click_handler(ClickRecognizerRef recognizer, void *context){
  up_pressed=1;
  app_timer_register(100,timer_callback,NULL);
}
void settings_up_long_click_release_handler(ClickRecognizerRef recognizer, void *context){
  up_pressed=0;
}
void settings_down_long_click_handler(ClickRecognizerRef recognizer, void *context){
  down_pressed=1;
  app_timer_register(100,timer_callback,NULL);
}
void settings_down_long_click_release_handler(ClickRecognizerRef recognizer, void *context){
  down_pressed=0;
}

void settings_config_provider(Window *window) {
 // single click 
  window_single_click_subscribe(BUTTON_ID_UP, settings_up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN,settings_down_single_click_handler);
  window_long_click_subscribe(BUTTON_ID_UP,500,settings_up_long_click_handler,settings_up_long_click_release_handler);
  window_long_click_subscribe(BUTTON_ID_DOWN,500,settings_down_long_click_handler,settings_down_long_click_release_handler);
}

void set_threshold(int n_threshold){
  threshold=n_threshold;
}
int get_threshold(void){
  return threshold;
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_settings(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_set_click_config_provider(s_window, (ClickConfigProvider) settings_config_provider);
  update_threshold_value();
  window_stack_push(s_window, true);
}

void hide_settings(void) {
  window_stack_remove(s_window, true);
}
