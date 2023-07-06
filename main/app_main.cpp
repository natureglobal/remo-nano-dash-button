#include "pairing_code.h"

#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include <esp_matter.h>
#include <esp_matter_commissioner.h>
#include <esp_matter_console.h>
#include <esp_matter_controller_console.h>
#include <esp_matter_controller_cluster_command.h>
#include <esp_matter_ota.h>
#include <esp_route_hook.h>

#include <device.h>

#include <app_reset.h>

static constexpr uint32_t TARGET_NODE_ID = 1;
static constexpr uint16_t TARGET_ENDPOINT_ID = 2;

static const char *TAG = "app_main";

typedef void *app_driver_handle_t;

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;

static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::PublicEventTypes::kInterfaceIpAddressChanged:
        ESP_LOGI(TAG, "Interface IP Address changed");
        break;

    default:
        break;
    }
}

static void app_driver_button_toggle_cb(void*, void*)
{
    using namespace chip::app::Clusters;
    ESP_LOGI(TAG, "Toggle button pressed");
    chip::DeviceLayer::StackLock lock;
    controller::command_data_t *command_data = (controller::command_data_t *)esp_matter_mem_calloc(1, sizeof(controller::command_data_t));
    command_data->cluster_id = OnOff::Id;
    command_data->command_id = OnOff::Commands::Toggle::Id;
    command_data->command_data_count = 0;
    controller::cluster_command *cmd = chip::Platform::New<controller::cluster_command>(1, 2, command_data);
    cmd->send_command();
}

static app_driver_handle_t app_driver_button_init()
{
    /* Initialize button */
    button_config_t config = button_driver_get_config();
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, app_driver_button_toggle_cb, NULL);
    return (app_driver_handle_t)handle;
}

extern "C" void app_main()
{
    esp_err_t err = ESP_OK;
    /* Initialize the ESP NVS layer */
    nvs_flash_init();
    app_driver_handle_t button_handle = app_driver_button_init();
    app_reset_button_register(button_handle);

    /* Matter start */
    err = esp_matter::start(app_event_cb);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Matter start failed: %d", err);
    }

    esp_matter::console::diagnostics_register_commands();
    esp_matter::console::wifi_register_commands();
    esp_matter::console::init();

    esp_matter::lock::chip_stack_lock(portMAX_DELAY);
    esp_matter::commissioner::init(5580);
    esp_matter::lock::chip_stack_unlock();

    esp_matter::console::controller_register_commands();
    esp_matter::console::pairing_code_register_commands();
}
