#include "include/https.h"
#include "include/response-parser.h"
#include "include/config.h"

char response_payload[1024];

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char output_buffer[1024];  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);

            // *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
            // *  However, event handler can also be used in case chunked encoding is used.

            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                if (evt->user_data) {
                    memcpy(evt->user_data + output_len, evt->data, evt->data_len);
                } else {
                    output_len = 0;
                    memcpy(output_buffer + output_len, evt->data, evt->data_len);
                }
                output_len += evt->data_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            output_buffer[output_len] = '\0';
            printf("%s\n", output_buffer);
            sprintf(response_payload, "%s", output_buffer);
            extract_response();
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                output_len = 0;
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            break;
    }
    return ESP_OK;
}

void https_do_https_request() {

    esp_tls_init_global_ca_store();
    esp_tls_set_global_ca_store(&server_root_cert_pem_start, server_root_cert_pem_end - server_root_cert_pem_start);

    esp_http_client_config_t* config = (esp_http_client_config_t*)calloc(sizeof(esp_http_client_config_t), 1);
    config->host = REST_HOST;
    config->path = REST_ENDPOINT;
    config->transport_type = HTTP_TRANSPORT_OVER_SSL;
    config->event_handler = _http_event_handler;

    esp_http_client_handle_t client = esp_http_client_init(config);
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}