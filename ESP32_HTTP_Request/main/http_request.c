#include "http_request.h"

static const char *TAG = "http client";


http_handle_t* http_handle_new(char* web_server)
{
    http_handle_t* http_handle = malloc(sizeof(*http_handle));
    http_handle->web_server = web_server;
    return http_handle;
}

void http_make_request(http_handle_t* http_handle, char* web_server, char* web_port, char* web_path)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s;

    uint16_t rec_size = 0;
    char http_request[1024];

    uint8_t err = getaddrinfo(web_server, web_port, &hints, &res);

    if(err != 0 || res == NULL) {
        ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }

    /* Code to print the resolved IP.

        Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
    addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
    ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

    s = socket(res->ai_family, res->ai_socktype, 0);
    if(s < 0) {
        ESP_LOGE(TAG, "... Failed to allocate socket.");
        freeaddrinfo(res);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
    ESP_LOGI(TAG, "... allocated socket");

    if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
        ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
        close(s);
        freeaddrinfo(res);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        
    }

    ESP_LOGI(TAG, "... connected");
    freeaddrinfo(res);

    http_request_composer(http_request, web_server, web_port, web_path);

    if (write(s, http_request, strlen(http_request)) < 0) {
        ESP_LOGE(TAG, "... socket send failed");
        close(s);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        
    }
    ESP_LOGI(TAG, "... socket send success");

    struct timeval receiving_timeout;
    receiving_timeout.tv_sec = 5;
    receiving_timeout.tv_usec = 0;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
            sizeof(receiving_timeout)) < 0) {
        ESP_LOGE(TAG, "... failed to set socket receiving timeout");
        close(s);
        vTaskDelay(4000 / portTICK_PERIOD_MS);
        
    }
    ESP_LOGI(TAG, "... set socket receiving timeout success");

    /* Read HTTP response */

    http_handle->rec_strlen = read(s, http_handle->recv_buf, sizeof(http_handle->recv_buf)-1);
    // for(int i = 0; i < http_handle->rec_strlen; i++) {
    //     putchar(http_handle->recv_buf[i]);
    // }

    ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", rec_size, errno);

    close(s);        
    
}

void http_request_composer(char* request_str, char* web_server, char* web_port, char* web_path)
{
    uint16_t data_len = strlen(web_server) + strlen(web_port) + strlen(web_path);
    char request[1024] = "GET ";
    strcat(request, web_path);
    strcat(request, " HTTP/1.0\r\n");
    strcat(request, "Host: ");
    strcat(request, web_server);
    strcat(request, ":");
    strcat(request, web_port);
    strcat(request, "\r\n");
    strcat(request, "User-Agent: esp-idf/1.0 esp32\r\n");
    strcat(request, "\r\n");

    for(uint16_t i=0; i<data_len + 128; i++){
        request_str[i] = request[i];
    }
}
