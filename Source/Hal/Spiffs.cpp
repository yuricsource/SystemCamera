/*
 * Wdt.cpp
 *
 *  Created on: 3 Jul 2019
 *      Author: yuri
 */

#include <Spiffs.h>

namespace Hal
{

Spiffs::Spiffs()
{
}

Spiffs::~Spiffs()
{
}

bool Spiffs::IsMounted()
{
    return _isMounted;
}

void Spiffs::Unmount()
{
    _memoryFree = 0;
    _memoryUsed = 0;
    _isMounted = false;
    esp_vfs_spiffs_unregister(NULL);
}

bool Spiffs::Mount()
{
    //printf("Initializing SPIFFS\n");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            printf("Failed to mount or format filesystem\n");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            printf("Failed to find SPIFFS partition\n");
        }
        else
        {
            printf("Failed to initialize SPIFFS (%s)\n", esp_err_to_name(ret));
        }
        return false;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK)
    {
        printf("Failed to get SPIFFS partition information (%s)\n", esp_err_to_name(ret));
        return false;
    }

    _memoryFree = total;
    _memoryUsed = used;
    _isMounted = true;

    return true;
}

} // namespace Hal
