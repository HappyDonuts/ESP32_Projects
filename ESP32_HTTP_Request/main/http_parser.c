#include "http_parser.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) 
{
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

uint8_t http_rec_parser(char* json_data, uint16_t json_strlen, uint16_t n_tokens_max) 
{
    uint16_t tokens_used;
    jsmn_parser p;
    jsmntok_t t[n_tokens_max]; 
    jsmn_init(&p);
    tokens_used = jsmn_parse(&p, json_data, json_strlen, t,
                    sizeof(t) / sizeof(t[0]));
    if (tokens_used < 0) {
    printf("Failed to parse JSON: %d\n", tokens_used);
    }
    printf("tokens_used = %d\n", tokens_used);
    /* Assume the top-level element is an object */
    if (tokens_used < 1 || t[0].type != JSMN_OBJECT) {
    printf("Object expected\n");
    }

    for (uint16_t i = 1; i < tokens_used; i++) {
        if (jsoneq(json_data, &t[i], "temp") == 0) {
            /* We may use strndup() to fetch string value */
            printf("- Temperature: %.*s °C\n", t[i + 1].end - t[i + 1].start,
           json_data + t[i + 1].start);
        } else if (jsoneq(json_data, &t[i], "temp_max") == 0) {
      /* We may additionally check if the value is either "true" or "false" */
      printf("- temp_max: %.*s °C\n", t[i + 1].end - t[i + 1].start,
             json_data + t[i + 1].start);
      i++;
    } else if (jsoneq(json_data, &t[i], "temp_min") == 0) {
      /* We may want to do strtol() here to get numeric value */
      printf("- temp_min: %.*s °C\n", t[i + 1].end - t[i + 1].start,
             json_data + t[i + 1].start);
      i++;
    } 
    // else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
//       int j;
//       printf("- Groups:\n");
//       if (t[i + 1].type != JSMN_ARRAY) {
//         continue; /* We expect groups to be an array of strings */
//       }
//       for (j = 0; j < t[i + 1].size; j++) {
//         jsmntok_t *g = &t[i + j + 2];
//         printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
//       }
//       i += t[i + 1].size + 1;
//     } else {
//       printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
//              JSON_STRING + t[i].start);
//     }
        
    }

  return EXIT_SUCCESS;
}