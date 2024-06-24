#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <soc.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "./include/ble.h"

static uint8_t mfg_data[] = { 0xff, 0xff, 0x00 };

#define BT_UUID_MY_SERVICE      BT_UUID_DECLARE_128(MY_SERVICE_UUID)
#define BT_UUID_MY_SERVICE_RX   BT_UUID_DECLARE_128(RX_CHARACTERISTIC_UUID)
#define BT_UUID_MY_SERVICE_TX   BT_UUID_DECLARE_128(TX_CHARACTERISTIC_UUID)

#define MAX_TRANSMIT_SIZE 240

uint8_t data_rx[MAX_TRANSMIT_SIZE];
uint8_t data_tx[MAX_TRANSMIT_SIZE];

static const struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 3),
};

static const struct bt_data sd[] =
    {
        BT_DATA_BYTES(BT_DATA_UUID128_ALL, MY_SERVICE_UUID),
    };

struct bt_conn *my_connection;
struct k_sem ble_init_ok;

int my_service_init(void)
{
    int err = 0;

    memset(&data_rx, 0, MAX_TRANSMIT_SIZE);
    memset(&data_tx, 0, MAX_TRANSMIT_SIZE);

    return err;
}

static ssize_t on_receive(struct bt_conn *conn,
                          const struct bt_gatt_attr *attr,
                          const void *buf,
                          uint16_t len,
                          uint16_t offset,
                          uint8_t flags)
{
    const uint8_t *buffer = buf;

    printk("Received data, handle %d, conn %p, data: 0x", attr->handle, conn);
    for (uint8_t i = 0; i < len; i++)
    {
        printk("%02X", buffer[i]);
    }
    printk("\n");

    return len;
}

static void on_sent(struct bt_conn *conn, void *user_data)
{
    ARG_UNUSED(user_data);

    const bt_addr_le_t *addr = bt_conn_get_dst(conn);

    printk("Data sent to Address 0x %02X %02X %02X %02X %02X %02X \n", addr->a.val[0],
                                                                     addr->a.val[1],
                                                                     addr->a.val[2],
                                                                     addr->a.val[3],
                                                                     addr->a.val[4],
                                                                     addr->a.val[5]);
}

void on_cccd_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    ARG_UNUSED(attr);
    switch (value)
    {
    case BT_GATT_CCC_NOTIFY:
        // Start sending stuff!
        break;

    case BT_GATT_CCC_INDICATE:
        // Start sending stuff via indications
        break;

    case 0:
        // Stop sending stuff
        break;

    default:
        printk("Error, CCCD has been set to an invalid value");
    }
}

BT_GATT_SERVICE_DEFINE(my_service,
                       BT_GATT_PRIMARY_SERVICE(BT_UUID_MY_SERVICE),
                       BT_GATT_CHARACTERISTIC(BT_UUID_MY_SERVICE_RX,
                                              BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                                              BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                                              NULL, on_receive, NULL),
                       BT_GATT_CHARACTERISTIC(BT_UUID_MY_SERVICE_TX,
                                              BT_GATT_CHRC_NOTIFY,
                                              BT_GATT_PERM_READ,
                                              NULL, NULL, NULL),
                       BT_GATT_CCC(on_cccd_changed,
                                   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE), );

void my_service_send(struct bt_conn *conn, const uint32_t *data, uint16_t len)
{
    const struct bt_gatt_attr *attr = &my_service.attrs[3];

    struct bt_gatt_notify_params params =
        {
            .uuid = BT_UUID_MY_SERVICE_TX,
            .attr = attr,
            .data = data,
            .len = len,
            .func = on_sent};

    if (bt_gatt_is_subscribed(conn, attr, BT_GATT_CCC_NOTIFY))
    {
        if (bt_gatt_notify_cb(conn, &params))
        {
            printk("Error, unable to send notification\n");
        }
    }
    else
    {
        printk("Warning, notification not enabled on the selected attribute\n");
    }
}

static void connected(struct bt_conn *conn, uint8_t err)
{
    struct bt_conn_info info;
    char addr[BT_ADDR_LE_STR_LEN];

    my_connection = conn;

    if (err)
    {
        printk("Connection failed (err %u)\n", err);
        return;
    }
    else if (bt_conn_get_info(conn, &info))
    {
        printk("Could not parse connection info\n");
    }
    else
    {
        bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

        printk("Connection established!     \n\
        Connected to: %s                    \n\
        Role: %u                            \n\
        Connection interval: %u             \n\
        Slave latency: %u                   \n\
        Connection supervisory timeout: %u  \n",
               addr, info.role, info.le.interval, info.le.latency, info.le.timeout);
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected (reason %u)\n", reason);
}

static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
    return true;
}

static void le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout)
{
    struct bt_conn_info info;
    char addr[BT_ADDR_LE_STR_LEN];

    if (bt_conn_get_info(conn, &info))
    {
        printk("Could not parse connection info\n");
    }
    else
    {
        bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

        printk("Connection parameters updated!  \n\
        Connected to: %s                        \n\
        New Connection Interval: %u             \n\
        New Slave Latency: %u                   \n\
        New Connection Supervisory Timeout: %u  \n",
               addr, info.le.interval, info.le.latency, info.le.timeout);
    }
}

static struct bt_conn_cb conn_callbacks =
    {
        .connected = connected,
        .disconnected = disconnected,
        .le_param_req = le_param_req,
        .le_param_updated = le_param_updated};

static void bt_ready(int err)
{
    if (err)
    {
        printk("BLE init failed with error code %d\n", err);
        return;
    }

    bt_conn_cb_register(&conn_callbacks);

    err = my_service_init();
    if (err)
    {
        printk("Failed to init my_service (err:%d)\n", err);
        return;
    }

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err)
    {
        printk("Advertising failed to start (err %d)\n", err);
        return;
    }

    printk("Advertising successfully started\n");

    k_sem_give(&ble_init_ok);
}

static void error(void)
{
    while (true) {
        printk("Error!\n");
        /* Spin for ever */
        k_sleep(K_MSEC(1000)); //1000ms
    }
}

int ble_init(void)
{
    int err;

    k_sem_init(&ble_init_ok, 0, 1);
    err = bt_enable(bt_ready);

    if (err)
    {
        printk("BLE initialization failed\n");
        return -1;
    }

    err = k_sem_take(&ble_init_ok, K_MSEC(500));

    if (!err)
    {
        printk("Bluetooth initialized\n");
    }
    else
    {
        printk("BLE initialization did not complete in time\n");
        return -1;
    }
    return 0;
}