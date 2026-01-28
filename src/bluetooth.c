#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include <stdbool.h>
#include "bluetooth.h"

static void set_powered(bool on) {
    DBusError err;
    DBusConnection *bus;
    DBusMessage *msg;
    DBusMessageIter args, variant;

    dbus_error_init(&err);

    bus = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (!bus) {
        fprintf(stderr, "dbus_bus_get: %s\n", err.message);
        exit(1);
    }

    msg = dbus_message_new_method_call(
        "org.bluez",                          // service
        "/org/bluez/hci0",                    // adapter
        "org.freedesktop.DBus.Properties",    // interface
        "Set"                                 // method
    );

    const char *iface = "org.bluez.Adapter1";
    const char *prop  = "Powered";

    dbus_message_iter_init_append(msg, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &iface);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &prop);

    dbus_message_iter_open_container(
        &args, DBUS_TYPE_VARIANT, "b", &variant
    );
    dbus_message_iter_append_basic(&variant, DBUS_TYPE_BOOLEAN, &on);
    dbus_message_iter_close_container(&args, &variant);

    if (!dbus_connection_send(bus, msg, NULL)) {
        fprintf(stderr, "send failed\n");
        exit(1);
    }

    dbus_connection_flush(bus);
    dbus_message_unref(msg);
}


int bluetooth_on(void){
    printf("bt on\n"); 
    return 0;
}
int bluetooth_off(void){
    printf("bt off\n"); 
    return 0;
}
int bluetooth_connect(void){
    printf("bt con\n"); 
    return 0;
}
int bluetooth_disconnect(void){
    printf("bt discon\n"); 
    return 0;
}

