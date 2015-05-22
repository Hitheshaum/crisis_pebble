#include "clock.h"
#include <pebble.h>
#include "notify.h"

static bool is_launched=false;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_roboto_bold_subset_49;
static GFont s_res_roboto_condensed_21;
static TextLayer *time_tl;
static TextLayer *date_tl;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, 0);
  
  s_res_roboto_bold_subset_49 = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // time_tl
  time_tl = text_layer_create(GRect(1, 46, 144, 57));
  text_layer_set_background_color(time_tl, GColorBlack);
  text_layer_set_text_color(time_tl, GColorWhite);
  text_layer_set_text(time_tl, "00:00");
  text_layer_set_text_alignment(time_tl, GTextAlignmentCenter);
  text_layer_set_font(time_tl, s_res_roboto_bold_subset_49);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_tl);
  
  // date_tl
  date_tl = text_layer_create(GRect(32, 105, 83, 29));
  text_layer_set_background_color(date_tl, GColorBlack);
  text_layer_set_text_color(date_tl, GColorWhite);
  text_layer_set_text(date_tl, "May 22");
  text_layer_set_text_alignment(date_tl, GTextAlignmentCenter);
  text_layer_set_font(date_tl, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)date_tl);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(time_tl);
  text_layer_destroy(date_tl);
}
// END AUTO-GENERATED UI CODE


void clock_select_single_click_handler(ClickRecognizerRef recognizer, void *context){
  show_notify();
}

void clock_back_single_click_handler(ClickRecognizerRef recognizer, void *context){
}

void clock_config_provider(Window *window) {
//   window_single_click_subscribe(BUTTON_ID_BACK, clock_back_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, clock_select_single_click_handler);
}
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";
  static char date_text[] = "Xxxxxxxxx 00";
  char *time_format;
  if (!tick_time) {
    time_t now = time(NULL);
    tick_time = localtime(&now);
  }
  // TODO: Only update the date when it's changed.
  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
  text_layer_set_text(date_tl, date_text);
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }
  strftime(time_text, sizeof(time_text), time_format, tick_time);
  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }
  text_layer_set_text(time_tl, time_text);
}

static void worker_message_handler(uint16_t type, AppWorkerMessage *data) {
  if(is_launched==false){
    show_notify();
  }
  is_launched=true;
}

static void handle_window_unload(Window* window) {
  tick_timer_service_unsubscribe();
  app_worker_message_unsubscribe();
  destroy_ui();
}

void show_clock(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  handle_minute_tick(NULL, MINUTE_UNIT);
  window_set_fullscreen(s_window, true);
  window_set_click_config_provider(s_window, (ClickConfigProvider) clock_config_provider);
  app_worker_message_subscribe(worker_message_handler);
  is_launched=false;
  window_stack_push(s_window, true);
}

void hide_clock(void) {
  window_stack_remove(s_window, true);
}

void reset_is_launched(void){
  is_launched=false;
}
