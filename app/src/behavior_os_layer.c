#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/ble.h>
#include <zmk/keymap.h>

#define MAC_LAYER 1

static void update_os_layers(uint8_t profile_index) {
    switch (profile_index) {
    case 0:
    case 2:
    case 4:
        zmk_keymap_layer_activate(MAC_LAYER);
        break;
    default:
        zmk_keymap_layer_deactivate(MAC_LAYER);
        break;
    }
}

static int os_layer_listener_cb(const zmk_event_t *eh) {
    uint8_t profile = zmk_ble_active_profile_index();
    update_os_layers(profile);
    return 0;
}

ZMK_LISTENER(os_layer_listener, os_layer_listener_cb);
ZMK_SUBSCRIPTION(os_layer_listener, zmk_ble_active_profile_changed);

static int behavior_os_layer_init(void) {
    uint8_t profile = zmk_ble_active_profile_index();
    update_os_layers(profile);
    return 0;
}

SYS_INIT(behavior_os_layer_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
