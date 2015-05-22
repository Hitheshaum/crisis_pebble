#include "exit.h"
#include <pebble.h>
#include "options.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_24_bold;
static TextLayer *ok_tl;
static TextLayer *cancel_tl;
static TextLayer *exit_message_tl;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, true);
  
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // ok_tl
  ok_tl = text_layer_create(GRect(4, 6, 37, 24));
  text_layer_set_text(ok_tl, "< OK");
  text_layer_set_font(ok_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)ok_tl);
  
  // cancel_tl
  cancel_tl = text_layer_create(GRect(66, 135, 71, 24));
  text_layer_set_text(cancel_tl, "Cancel >");
  text_layer_set_text_alignment(cancel_tl, GTextAlignmentRight);
  text_layer_set_font(cancel_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)cancel_tl);
  
  // exit_message_tl
  exit_message_tl = text_layer_create(GRect(10, 42, 125, 91));
  text_layer_set_background_color(exit_message_tl, GColorBlack);
  text_layer_set_text_color(exit_message_tl, GColorWhite);
  text_layer_set_text(exit_message_tl, "Fall detection will be deactivated");
  text_layer_set_text_alignment(exit_message_tl, GTextAlignmentCenter);
  text_layer_set_font(exit_message_tl, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)exit_message_tl);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(ok_tl);
  text_layer_destroy(cancel_tl);
  text_layer_destroy(exit_message_tl);
}
// END AUTO-GENERATED UI CODE

void exit_down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  show_options();
  hide_exit();
}
void exit_back_single_click_handler(ClickRecognizerRef recognizer, void *context){
  app_worker_kill();
  window_stack_pop_all(true);
}
void exit_config_provider(Window *window) {
 // single click 
  window_single_click_subscribe(BUTTON_ID_DOWN, exit_down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, exit_back_single_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_exit(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_set_click_config_provider(s_window, (ClickConfigProvider) exit_config_provider);
  window_stack_push(s_window, true);
}

void hide_exit(void) {
  window_stack_remove(s_window, true);
}
