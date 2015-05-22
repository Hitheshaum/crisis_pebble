#include <pebble_worker.h>
#include "fall_detector.h"
#define THRESHOLD_KEY 1
static int threshold=0;

static uint32_t abs_cur=0,abs_prev=0;

static void send_to_foreground() {
  // Construct a data packet
  AppWorkerMessage msg_data = {
    .data0 = 1
  };
  // Send the data to the foreground app
  app_worker_send_message(1, &msg_data);
}

static void data_handler(AccelData *data, uint32_t num_samples) {  
  abs_cur=(((data->x)*(data->x))+((data->y)*(data->y))+((data->z)*(data->z)));
  if(abs(abs_cur-abs_prev)>(20000000+threshold*100000)){
    worker_launch_app();
    send_to_foreground();
  }
  abs_prev=abs_cur;
}

  
static void init() {
  // Initialize your worker here
  uint32_t num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_50HZ);
//   threshold=persist_read_int(THRESHOLD_KEY);

}

static void deinit() {
  // Deinitialize your worker here
  accel_data_service_unsubscribe();
}

int main(void) {
  init();
  worker_event_loop();
  deinit();
}