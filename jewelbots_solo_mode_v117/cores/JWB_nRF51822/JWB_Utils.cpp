#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Arduino.h"
#include "JWB_Utils.h"
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
#include "app_scheduler.h"
#include "nrf_soc.h"
#include "app_error.h"
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


uint8_t get_random_int(uint8_t min_value, uint8_t max_value){
  uint32_t err_code;
  uint8_t num_rand_bytes_available;
  uint8_t rando_num;
  uint8_t temp;
  uint8_t value_in_range;

  if (min_value > max_value){
    temp = max_value;
    max_value = min_value;
    min_value = temp;
  } else if (min_value == max_value){
    return min_value;
  }
  err_code = sd_rand_application_bytes_available_get(&num_rand_bytes_available);
  APP_ERROR_CHECK(err_code);
  if(num_rand_bytes_available > 0) {
    err_code = sd_rand_application_vector_get(&rando_num, 1);
    APP_ERROR_CHECK(err_code);
  }

  value_in_range = min_value + rando_num / (255 / (max_value - min_value + 1) + 1);
  return value_in_range;
}
