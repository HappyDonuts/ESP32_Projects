#include <string.h>
#include "esp_system.h"
#include "jsmn.h"

uint8_t http_rec_parser(char* json_data, uint16_t json_strlen, uint16_t n_tokens_max);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);