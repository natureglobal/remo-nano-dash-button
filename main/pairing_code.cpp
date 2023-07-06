#include "pairing_code.h"

#include <esp_matter_controller_pairing_command.h>
#include <esp_matter_controller_console.h>
#include <esp_matter_controller_utils.h>

#include <lib/core/CHIPCore.h>
#include <lib/shell/Commands.h>
#include <lib/shell/Engine.h>
#include <lib/shell/commands/Help.h>
#include <setup_payload/ManualSetupPayloadParser.h>

#include <esp_log.h>

const static char *TAG = "pairing_code";

// matter esp pairing-code onnetwork 1 21900752520

namespace esp_matter {
namespace console {
static engine pairing_code_console;

static esp_err_t pairing_code_help_handler(int argc, char **argv)
{
    pairing_code_console.for_each_command(print_description, nullptr);
    return ESP_OK;
}

static esp_err_t pairing_code_onnetwork_handler(int argc, char **argv)
{
    ESP_LOGI(TAG, "pairing-code onnetwork");
    if (argc != 2) {
        return ESP_ERR_INVALID_ARG;
    }

    const uint64_t nodeId = string_to_uint64(argv[0]);
    chip::SetupPayload payload;
    chip::ManualSetupPayloadParser(argv[1]).populatePayload(payload);
    const uint32_t pincode = payload.setUpPINCode;

    return controller::pairing_on_network(nodeId, pincode);
}

static esp_err_t pairing_code_dispatch(int argc, char **argv)
{
    if (argc == 0) {
        return pairing_code_help_handler(argc, argv);
    }
    return pairing_code_console.exec_command(argc, argv);
}

esp_err_t pairing_code_register_commands()
{
    // Subcommands for root command: `pairing-code <subcommand>`
    const static command_t pairing_code_sub_commands[] = {
        {
            .name = "help",
            .description = "print this page",
            .handler = pairing_code_help_handler,
        },
        {
            .name = "onnetwork",
            .description = "Pairing a node.\n"
                           "\tUsage: pairing-code onnetwork [nodeid] [MPC]",
            .handler = pairing_code_onnetwork_handler,
        },
    };

    const static command_t pairing_code_command = {
        .name = "pairing-code",
        .description = "Pairing with code commands. Usage: matter esp pairing-code [command_name]",
        .handler = pairing_code_dispatch,
    };
    // Register the pairing-code commands
    pairing_code_console.register_commands(pairing_code_sub_commands, sizeof(pairing_code_sub_commands) / sizeof(command_t));
    return add_commands(&pairing_code_command, 1);
}

} // namespace console
} // namespace esp_matter
