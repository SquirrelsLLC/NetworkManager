/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2018 - 2019 Red Hat, Inc.
 */

#include "libnm-client-impl/nm-default-libnm.h"

#include "nm-wifi-p2p-peer.h"
#include "nm-device-wifi-p2p.h"

#include "libnm-glib-aux/nm-dbus-aux.h"
#include "nm-setting-connection.h"
#include "nm-setting-wifi-p2p.h"
#include "nm-utils.h"
#include "nm-object-private.h"
#include "libnm-core-intern/nm-core-internal.h"
#include "nm-dbus-helpers.h"

/*****************************************************************************/

NM_GOBJECT_PROPERTIES_DEFINE_BASE(PROP_PEERS, PROP_GROUP, PROP_PIN, );

enum {
    PEER_ADDED,
    PEER_REMOVED,
    PIN_CHANGED,
    GROUP_ADDED,
    GROUP_REMOVED,

    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = {0};

typedef struct {
    NMLDBusPropertyAO peers;
    char             *group;
    char             *pin;
} NMDeviceWifiP2PPrivate;

struct _NMDeviceWifiP2P {
    NMDevice               parent;
    NMDeviceWifiP2PPrivate _priv;
};

struct _NMDeviceWifiP2PClass {
    NMDeviceClass parent;
};

G_DEFINE_TYPE(NMDeviceWifiP2P, nm_device_wifi_p2p, NM_TYPE_DEVICE)

#define NM_DEVICE_WIFI_P2P_GET_PRIVATE(self) \
    _NM_GET_PRIVATE(self, NMDeviceWifiP2P, NM_IS_DEVICE_WIFI_P2P, NMDevice, NMObject)

/*****************************************************************************/

/**
 * nm_device_wifi_p2p_get_hw_address: (skip)
 * @device: a #NMDeviceWifiP2P
 *
 * Gets the actual hardware (MAC) address of the #NMDeviceWifiP2P
 *
 * Returns: the actual hardware address. This is the internal string used by the
 * device, and must not be modified.
 *
 * Since: 1.16
 *
 * Deprecated: 1.24: Use nm_device_get_hw_address() instead.
 **/
const char *
nm_device_wifi_p2p_get_hw_address(NMDeviceWifiP2P *device)
{
    g_return_val_if_fail(NM_IS_DEVICE_WIFI_P2P(device), NULL);

    return nm_device_get_hw_address(NM_DEVICE(device));
}

/**
 * nm_device_wifi_p2p_get_peers:
 * @device: a #NMDeviceWifiP2P
 *
 * Gets all the found peers of the #NMDeviceWifiP2P.
 *
 * Returns: (element-type NMWifiP2PPeer): a #GPtrArray containing all the
 *          found #NMWifiP2PPeers.
 * The returned array is owned by the client and should not be modified.
 *
 * Since: 1.16
 **/
const GPtrArray *
nm_device_wifi_p2p_get_peers(NMDeviceWifiP2P *device)
{
    g_return_val_if_fail(NM_IS_DEVICE_WIFI_P2P(device), NULL);

    return nml_dbus_property_ao_get_objs_as_ptrarray(
        &NM_DEVICE_WIFI_P2P_GET_PRIVATE(device)->peers);
}

/**
 * nm_device_wifi_p2p_get_peer_by_path:
 * @device: a #NMDeviceWifiP2P
 * @path: the object path of the peer
 *
 * Gets a #NMWifiP2PPeer by path.
 *
 * Returns: (transfer none): the peer or %NULL if none is found.
 *
 * Since: 1.42
 **/
NMWifiP2PPeer *
nm_device_wifi_p2p_get_peer_by_path(NMDeviceWifiP2P *device, const char *path)
{
    const GPtrArray *peers;
    int              i;
    NMWifiP2PPeer   *peer = NULL;

    g_return_val_if_fail(NM_IS_DEVICE_WIFI_P2P(device), NULL);
    g_return_val_if_fail(path != NULL, NULL);

    peers = nm_device_wifi_p2p_get_peers(device);
    if (!peers)
        return NULL;

    for (i = 0; i < peers->len; i++) {
        NMWifiP2PPeer *candidate = g_ptr_array_index(peers, i);
        if (!strcmp(nm_object_get_path(NM_OBJECT(candidate)), path)) {
            peer = candidate;
            break;
        }
    }

    return peer;
}

/**
 * nm_device_wifi_p2p_get_pin:
 * @device: a #NMDeviceWifiP2P
 *
 * Gets the latest PIN that has been provisioned for P2P connection security.
 *
 * Returns: (transfer none): the PIN or %NULL if none is found.
 *
 * Since: 1.36
 **/
const char *
nm_device_wifi_p2p_get_pin(NMDeviceWifiP2P *device)
{
    g_return_val_if_fail(NM_IS_DEVICE_WIFI_P2P(device), NULL);

    return _nml_coerce_property_str_not_empty(NM_DEVICE_WIFI_P2P_GET_PRIVATE(device)->pin);
}

static const char *
_nm_device_wifi_p2p_get_group(NMDeviceWifiP2P *device)
{
    g_return_val_if_fail(NM_IS_DEVICE_WIFI_P2P(device), NULL);

    return _nml_coerce_property_str_not_empty(NM_DEVICE_WIFI_P2P_GET_PRIVATE(device)->group);
}

/**
 * nm_device_wifi_p2p_start_find:
 * @device: a #NMDeviceWifiP2P
 * @options: (nullable): optional options passed to StartFind.
 * @cancellable: a #GCancellable, or %NULL
 * @callback: a #GAsyncReadyCallback, or %NULL
 * @user_data: user_data for @callback
 *
 * Request NM to search for Wi-Fi P2P peers on @device. Note that the call
 * returns immediately after requesting the find, and it may take some time
 * after that for peers to be found.
 *
 * The find operation will run for 30s by default. You can stop it earlier
 * using nm_device_p2p_wifi_stop_find().
 *
 * Since: 1.16
 **/
void
nm_device_wifi_p2p_start_find(NMDeviceWifiP2P    *device,
                              GVariant           *options,
                              GCancellable       *cancellable,
                              GAsyncReadyCallback callback,
                              gpointer            user_data)
{
    g_return_if_fail(NM_IS_DEVICE_WIFI_P2P(device));
    g_return_if_fail(!options || g_variant_is_of_type(options, G_VARIANT_TYPE_VARDICT));
    g_return_if_fail(!cancellable || G_IS_CANCELLABLE(cancellable));

    if (!options)
        options = nm_g_variant_singleton_aLsvI();

    _nm_client_dbus_call(_nm_object_get_client(device),
                         device,
                         nm_device_wifi_p2p_start_find,
                         cancellable,
                         callback,
                         user_data,
                         _nm_object_get_path(device),
                         NM_DBUS_INTERFACE_DEVICE_WIFI_P2P,
                         "StartFind",
                         g_variant_new("(@a{sv})", options),
                         G_VARIANT_TYPE("()"),
                         G_DBUS_CALL_FLAGS_NONE,
                         NM_DBUS_DEFAULT_TIMEOUT_MSEC,
                         nm_dbus_connection_call_finish_void_cb);
}

/**
 * nm_device_wifi_p2p_start_find_finish:
 * @device: a #NMDeviceWifiP2P
 * @result: the #GAsyncResult
 * @error: #GError return address
 *
 * Finish an operation started by nm_device_wifi_p2p_start_find().
 *
 * Returns: %TRUE if the call was successful
 *
 * Since: 1.16
 **/
gboolean
nm_device_wifi_p2p_start_find_finish(NMDeviceWifiP2P *device, GAsyncResult *result, GError **error)
{
    g_return_val_if_fail(NM_IS_DEVICE_WIFI_P2P(device), FALSE);
    g_return_val_if_fail(nm_g_task_is_valid(result, device, nm_device_wifi_p2p_start_find), FALSE);

    return g_task_propagate_boolean(G_TASK(result), error);
}

/**
 * nm_device_wifi_p2p_stop_find:
 * @device: a #NMDeviceWifiP2P
 * @cancellable: a #GCancellable, or %NULL
 * @callback: a #GAsyncReadyCallback, or %NULL
 * @user_data: user_data for @callback
 *
 * Request NM to stop any ongoing find operation for Wi-Fi P2P peers on @device.
 *
 * Since: 1.16
 **/
void
nm_device_wifi_p2p_stop_find(NMDeviceWifiP2P    *device,
                             GCancellable       *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer            user_data)
{
    g_return_if_fail(NM_IS_DEVICE_WIFI_P2P(device));
    g_return_if_fail(!cancellable || G_IS_CANCELLABLE(cancellable));

    _nm_client_dbus_call(_nm_object_get_client(device),
                         device,
                         nm_device_wifi_p2p_stop_find,
                         cancellable,
                         callback,
                         user_data,
                         _nm_object_get_path(device),
                         NM_DBUS_INTERFACE_DEVICE_WIFI_P2P,
                         "StopFind",
                         g_variant_new("()"),
                         G_VARIANT_TYPE("()"),
                         G_DBUS_CALL_FLAGS_NONE,
                         NM_DBUS_DEFAULT_TIMEOUT_MSEC,
                         nm_dbus_connection_call_finish_void_cb);
}

/**
 * nm_device_wifi_p2p_stop_find_finish:
 * @device: a #NMDeviceWifiP2P
 * @result: the #GAsyncResult
 * @error: #GError return address
 *
 * Finish an operation started by nm_device_wifi_p2p_stop_find().
 *
 * Returns: %TRUE if the call was successful
 *
 * Since: 1.16
 **/
gboolean
nm_device_wifi_p2p_stop_find_finish(NMDeviceWifiP2P *device, GAsyncResult *result, GError **error)
{
    g_return_val_if_fail(NM_IS_DEVICE_WIFI_P2P(device), FALSE);
    g_return_val_if_fail(nm_g_task_is_valid(result, device, nm_device_wifi_p2p_stop_find), FALSE);

    return g_task_propagate_boolean(G_TASK(result), error);
}

static gboolean
connection_compatible(NMDevice *device, NMConnection *connection, GError **error)
{
    if (!NM_DEVICE_CLASS(nm_device_wifi_p2p_parent_class)
             ->connection_compatible(device, connection, error))
        return FALSE;

    if (!nm_connection_is_type(connection, NM_SETTING_WIFI_P2P_SETTING_NAME)) {
        g_set_error_literal(error,
                            NM_DEVICE_ERROR,
                            NM_DEVICE_ERROR_INCOMPATIBLE_CONNECTION,
                            _("The connection was not a Wi-Fi P2P connection."));
        return FALSE;
    }

    return TRUE;
}

static GType
get_setting_type(NMDevice *device)
{
    return NM_TYPE_SETTING_WIRELESS;
}

static const char *
get_type_description(NMDevice *device)
{
    return "wifi-p2p";
}

/*****************************************************************************/

static void
_property_ao_notify_changed_peers_cb(NMLDBusPropertyAO *pr_ao,
                                     NMClient          *client,
                                     NMObject          *nmobj,
                                     gboolean           is_added /* or else removed */)
{
    _nm_client_notify_event_queue_emit_obj_signal(client,
                                                  G_OBJECT(pr_ao->owner_dbobj->nmobj),
                                                  nmobj,
                                                  is_added,
                                                  10,
                                                  is_added ? signals[PEER_ADDED]
                                                           : signals[PEER_REMOVED]);
}

/*****************************************************************************/

static NMLDBusNotifyUpdatePropFlags
_notify_update_prop_group(NMClient               *client,
                          NMLDBusObject          *dbobj,
                          const NMLDBusMetaIface *meta_iface,
                          guint                   dbus_property_idx,
                          GVariant               *value)
{
    NMDeviceWifiP2P        *self = NM_DEVICE_WIFI_P2P(dbobj->nmobj);
    NMDeviceWifiP2PPrivate *priv = NM_DEVICE_WIFI_P2P_GET_PRIVATE(self);
    const char             *new_group;
    gs_free char           *old_group = NULL;
    (void) client;
    (void) meta_iface;
    (void) dbus_property_idx;

    new_group = value ? nm_dbus_path_not_empty(g_variant_get_string(value, NULL)) : NULL;

    NML_NMCLIENT_LOG_D(client,
                       "[%s] group update: old=%s new=%s",
                       _nm_object_get_path(self),
                       priv->group ?: "/",
                       new_group ?: "/");

    if (nm_streq0(priv->group, new_group))
        return NML_DBUS_NOTIFY_UPDATE_PROP_FLAGS_NONE;

    old_group   = g_steal_pointer(&priv->group);
    priv->group = g_strdup(new_group);

    if (old_group)
        g_signal_emit(self, signals[GROUP_REMOVED], 0, old_group);
    if (priv->group)
        g_signal_emit(self, signals[GROUP_ADDED], 0, priv->group);

    NML_NMCLIENT_LOG_D(client, "[%s] group signals emitted", _nm_object_get_path(self));

    return NML_DBUS_NOTIFY_UPDATE_PROP_FLAGS_NOTIFY;
}

/*****************************************************************************/

static NMLDBusNotifyUpdatePropFlags
_notify_update_prop_pin(NMClient               *client,
                        NMLDBusObject          *dbobj,
                        const NMLDBusMetaIface *meta_iface,
                        guint                   dbus_property_idx,
                        GVariant               *value)
{
    NMDeviceWifiP2P        *self = NM_DEVICE_WIFI_P2P(dbobj->nmobj);
    NMDeviceWifiP2PPrivate *priv = NM_DEVICE_WIFI_P2P_GET_PRIVATE(self);
    const char             *new_pin;
    (void) client;
    (void) meta_iface;
    (void) dbus_property_idx;

    new_pin = value ? _nml_coerce_property_str_not_empty(g_variant_get_string(value, NULL)) : NULL;

    NML_NMCLIENT_LOG_D(client,
                       "[%s] pin update: old=%s new=%s",
                       _nm_object_get_path(self),
                       priv->pin ? "<set>" : "<unset>",
                       new_pin ? "<set>" : "<unset>");

    if (nm_streq0(priv->pin, new_pin))
        return NML_DBUS_NOTIFY_UPDATE_PROP_FLAGS_NONE;

    nm_strdup_reset(&priv->pin, new_pin);

    if (priv->pin)
        g_signal_emit(self, signals[PIN_CHANGED], 0, priv->pin);

    NML_NMCLIENT_LOG_D(client,
                       "[%s] pin signal emitted=%s",
                       _nm_object_get_path(self),
                       priv->pin ? "yes" : "no");

    return NML_DBUS_NOTIFY_UPDATE_PROP_FLAGS_NOTIFY;
}

/*****************************************************************************/

static void
get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    NMDeviceWifiP2P *self = NM_DEVICE_WIFI_P2P(object);

    switch (prop_id) {
    case PROP_PEERS:
        g_value_take_boxed(value, _nm_utils_copy_object_array(nm_device_wifi_p2p_get_peers(self)));
        break;
    case PROP_GROUP:
        g_value_set_string(value, _nm_device_wifi_p2p_get_group(self));
        break;
    case PROP_PIN:
        g_value_set_string(value, nm_device_wifi_p2p_get_pin(self));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}

/*****************************************************************************/

static void
nm_device_wifi_p2p_init(NMDeviceWifiP2P *device)
{}

static void
finalize(GObject *object)
{
    NMDeviceWifiP2PPrivate *priv = NM_DEVICE_WIFI_P2P_GET_PRIVATE(object);

    g_free(priv->group);
    g_free(priv->pin);
    G_OBJECT_CLASS(nm_device_wifi_p2p_parent_class)->finalize(object);
}

const NMLDBusMetaIface _nml_dbus_meta_iface_nm_device_wifip2p = NML_DBUS_META_IFACE_INIT_PROP(
    NM_DBUS_INTERFACE_DEVICE_WIFI_P2P,
    nm_device_wifi_p2p_get_type,
    NML_DBUS_META_INTERFACE_PRIO_INSTANTIATE_30,
    NML_DBUS_META_IFACE_DBUS_PROPERTIES(
        NML_DBUS_META_PROPERTY_INIT_FCN("Group",
                                        PROP_GROUP,
                                        "o",
                                        _notify_update_prop_group),
        NML_DBUS_META_PROPERTY_INIT_FCN("HwAddress",
                                        0,
                                        "s",
                                        _nm_device_notify_update_prop_hw_address),
        NML_DBUS_META_PROPERTY_INIT_AO_PROP("Peers",
                                            PROP_PEERS,
                                            NMDeviceWifiP2P,
                                            _priv.peers,
                                            nm_wifi_p2p_peer_get_type,
                                            .notify_changed_ao =
                                                _property_ao_notify_changed_peers_cb),
        NML_DBUS_META_PROPERTY_INIT_FCN("Pin",
                                        PROP_PIN,
                                        "s",
                                        _notify_update_prop_pin), ), );

static void
nm_device_wifi_p2p_class_init(NMDeviceWifiP2PClass *klass)
{
    GObjectClass  *object_class    = G_OBJECT_CLASS(klass);
    NMObjectClass *nm_object_class = NM_OBJECT_CLASS(klass);
    NMDeviceClass *device_class    = NM_DEVICE_CLASS(klass);

    object_class->get_property = get_property;
    object_class->finalize     = finalize;

    _NM_OBJECT_CLASS_INIT_PRIV_PTR_DIRECT(nm_object_class, NMDeviceWifiP2P);

    _NM_OBJECT_CLASS_INIT_PROPERTY_AO_FIELDS_1(nm_object_class, NMDeviceWifiP2PPrivate, peers);

    device_class->connection_compatible = connection_compatible;
    device_class->get_setting_type      = get_setting_type;
    device_class->get_type_description  = get_type_description;

    /**
     * NMDeviceWifiP2P:peers: (type GPtrArray(NMWifiP2PPeer))
     *
     * List of all Wi-Fi P2P peers the device can see.
     *
     * Since: 1.16
     **/
    obj_properties[PROP_PEERS] = g_param_spec_boxed(NM_DEVICE_WIFI_P2P_PEERS,
                                                    "",
                                                    "",
                                                    G_TYPE_PTR_ARRAY,
                                                    G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);

    obj_properties[PROP_GROUP] =
        g_param_spec_string(NM_DEVICE_WIFI_P2P_GROUP, "", "", NULL, G_PARAM_READABLE);

    /**
     * NMDeviceWifiP2P:pin:
     *
     * The most recently provisioned PIN for P2P connection security.
     *
     * Since: 1.36
     **/
    obj_properties[PROP_PIN] =
        g_param_spec_string(NM_DEVICE_WIFI_P2P_PIN, "", "", NULL, G_PARAM_READABLE);

    _nml_dbus_meta_class_init_with_properties(object_class,
                                              &_nml_dbus_meta_iface_nm_device_wifip2p);

    /**
     * NMDeviceWifiP2P::peer-added:
     * @device: the Wi-Fi P2P device that received the signal
     * @peer: the new access point
     *
     * Notifies that a #NMWifiP2PPeer is added to the Wi-Fi P2P device.
     *
     * Since: 1.16
     **/
    signals[PEER_ADDED] = g_signal_new("peer-added",
                                       G_OBJECT_CLASS_TYPE(object_class),
                                       G_SIGNAL_RUN_FIRST,
                                       0,
                                       NULL,
                                       NULL,
                                       g_cclosure_marshal_VOID__OBJECT,
                                       G_TYPE_NONE,
                                       1,
                                       G_TYPE_OBJECT);

    /**
     * NMDeviceWifiP2P::peer-removed:
     * @device: the Wi-Fi P2P device that received the signal
     * @peer: the removed access point
     *
     * Notifies that a #NMWifiP2PPeer is removed from the Wi-Fi P2P device.
     *
     * Since: 1.16
     **/
    signals[PEER_REMOVED] = g_signal_new("peer-removed",
                                         G_OBJECT_CLASS_TYPE(object_class),
                                         G_SIGNAL_RUN_FIRST,
                                         0,
                                         NULL,
                                         NULL,
                                         g_cclosure_marshal_VOID__OBJECT,
                                         G_TYPE_NONE,
                                         1,
                                         G_TYPE_OBJECT);

    /**
     * NMDeviceWifiP2P::pin-changed:
     * @device: the Wi-Fi P2P device that received the signal
     * @pin: the generated pin code
     *
     * Notifies that the PIN property changed to a new generated PIN.
     *
     * Since: 1.36
     **/
    signals[PIN_CHANGED] = g_signal_new("pin-changed",
                                            G_OBJECT_CLASS_TYPE(object_class),
                                            G_SIGNAL_RUN_FIRST,
                                            0,
                                            NULL,
                                            NULL,
                                            g_cclosure_marshal_VOID__STRING,
                                            G_TYPE_NONE,
                                            1,
                                            G_TYPE_STRING);

    signals[GROUP_ADDED] = g_signal_new("group-added",
                                        G_OBJECT_CLASS_TYPE(object_class),
                                        G_SIGNAL_RUN_FIRST,
                                        0,
                                        NULL,
                                        NULL,
                                        g_cclosure_marshal_VOID__STRING,
                                        G_TYPE_NONE,
                                        1,
                                        G_TYPE_STRING);

    signals[GROUP_REMOVED] = g_signal_new("group-removed",
                                        G_OBJECT_CLASS_TYPE(object_class),
                                        G_SIGNAL_RUN_FIRST,
                                        0,
                                        NULL,
                                        NULL,
                                        g_cclosure_marshal_VOID__STRING,
                                        G_TYPE_NONE,
                                        1,
                                        G_TYPE_STRING);
}
