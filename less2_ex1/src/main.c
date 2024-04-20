/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
// Include the header files of the BLE stack
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>

#include <dk_buttons_and_leds.h>

LOG_MODULE_REGISTER(Lesson2_Exercise1, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000

// Declare the Advertising packet
static const struct bt_data ad[] = {
    // Set the advertising flag BT_LE_AD_NO_BRDR to create a broadcaster with non-connectable advertising and indicate that classic bluetooth is not supported
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
    // Set the advertising packet data
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN)};

// Declare the URL data to include in the scan response
// 0x17 means 'https:' based on URI scheme name string mapping from Bluetooth SIG
static unsigned char url_data[] = {0x17, '/', '/', 'a', 'c', 'a', 'd', 'e', 'm', 'y', '.',
                                   'n', 'o', 'r', 'd', 'i', 'c', 's', 'e', 'm', 'i', '.',
                                   'c', 'o', 'm'};
/* Declare the scan response packet */
static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_URI, url_data, sizeof(url_data))};

void main(void)
{
        int blink_status = 0;
        int err;

        LOG_INF("Starting Lesson 2 - Exercise 1 \n");

        err = dk_leds_init();
        if (err)
        {
                LOG_ERR("LEDs init failed (err %d)\n", err);
                return;
        }
        // Enable the Bluetooth LE stack
        err = bt_enable(NULL);
        if (err)
        {
                LOG_ERR("Bluetooth init failed (err %d)\n", err);
                return;
        }
        LOG_INF("Bluetooth initialized\n");
        // Start advertising
        err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
                              sd, ARRAY_SIZE(sd));
        if (err)
        {
                LOG_ERR("Advertising failed to start (err %d)\n", err);
                return;
        }

        LOG_INF("Advertising successfully started\n");

        for (;;)
        {
                dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
                k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
        }
}