#include "options.h"
#include <pebble.h>
#include "exit.h"
#include "settings.h"
#include "notify.h"
#include "clock.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_24_bold;
static GFont s_res_gothic_18_bold;
static TextLayer *background_tl;
static TextLayer *clock_tl;
static TextLayer *off_tl;
static TextLayer *title_tl;
static TextLayer *settings_tl;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, true);
  
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // background_tl
  background_tl = text_layer_create(GRect(40, 56, 101, 54));
  text_layer_set_text(background_tl, "Run in    background >");
  text_layer_set_text_alignment(background_tl, GTextAlignmentRight);
  text_layer_set_font(background_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)background_tl);
  
  // clock_tl
  clock_tl = text_layer_create(GRect(81, 133, 59, 24));
  text_layer_set_text(clock_tl, "Clock >");
  text_layer_set_text_alignment(clock_tl, GTextAlignmentRight);
  text_layer_set_font(clock_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)clock_tl);
  
  // off_tl
  off_tl = text_layer_create(GRect(3, 31, 44, 24));
  text_layer_set_text(off_tl, "< OFF");
  text_layer_set_font(off_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)off_tl);
  
  // title_tl
  title_tl = text_layer_create(GRect(0, -3, 144, 21));
  text_layer_set_background_color(title_tl, GColorBlack);
  text_layer_set_text_color(title_tl, GColorWhite);
  text_layer_set_text(title_tl, "CRISIS");
  text_layer_set_text_alignment(title_tl, GTextAlignmentCenter);
  text_layer_set_font(title_tl, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)title_tl);
  
  // settings_tl
  settings_tl = text_layer_create(GRect(60, 19, 81, 29));
  text_layer_set_text(settings_tl, "Settings >");
  text_layer_set_text_alignment(settings_tl, GTextAlignmentRight);
  text_layer_set_font(settings_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)settings_tl);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(background_tl);
  text_layer_destroy(clock_tl);
  text_layer_destroy(off_tl);
  text_layer_destroy(title_tl);
  text_layer_destroy(settings_tl);
}
// END AUTO-GENERATED UI CODE

void options_back_single_click_handler(ClickRecognizerRef recognizer, void *context){
  hide_options();
  show_exit();
}
void options_up_single_click_handler(ClickRecognizerRef recognizer, void *context){
  show_settings();
}
void options_down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  app_worker_launch();
  show_clock();
}
void options_select_single_click_handler(ClickRecognizerRef recognizer, void *context){
  int status_w=app_worker_launch();
  static char buf[] = "100";
  snprintf(buf, sizeof(buf), "%d", status_w);
  APP_LOG(APP_LOG_LEVEL_DEBUG, buf);
  window_stack_pop_all(true);
}

void options_config_provider(Window *window) {
 // single click 
  window_single_click_subscribe(BUTTON_ID_BACK, options_back_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, options_up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN,options_down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT,options_select_single_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_options(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_set_click_config_provider(s_window, (ClickConfigProvider) options_config_provider);
  window_stack_push(s_window, true);
}

void hide_options(void) {
  window_stack_remove(s_window, true);
}

void init(void){
  if(launch_reason()==APP_LAUNCH_WORKER){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "launched by worker");
    show_notify();
//     app_worker_kill();
  } else{
    show_options();
    set_threshold(persist_read_int(THRESHOLD_KEY));
  }
}

void deinit(void){
//   hide_options();
  hide_exit();
  persist_write_int(THRESHOLD_KEY,get_threshold());
}

int main(void){
  init();
  app_event_loop();
  deinit();
}
