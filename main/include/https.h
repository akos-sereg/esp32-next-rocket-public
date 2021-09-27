#ifndef __https_h_included__
#define __https_h_included__

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_crt_bundle.h"
#include "esp_tls.h"
#include "networking.h"
#include "esp_http_client.h"

extern const char server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const char server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");

extern char response_payload[1024];
extern void https_do_https_request();

#endif