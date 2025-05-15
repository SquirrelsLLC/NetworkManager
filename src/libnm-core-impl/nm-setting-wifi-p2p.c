/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019 Red Hat, Inc.
 */

#include "libnm-core-impl/nm-default-libnm-core.h"

#include "nm-setting-wifi-p2p.h"

#include <net/ethernet.h>

#include "nm-utils.h"
#include "libnm-core-aux-intern/nm-common-macros.h"
#include "nm-core-enum-types.h"
#include "nm-utils-private.h"
#include "nm-setting-private.h"

/**
 * SECTION:nm-setting-wifi-p2p
 * @short_description: Describes connection properties for 802.11 Wi-Fi P2P networks
 *
 * The #NMSettingWifiP2P object is a #NMSetting subclass that describes properties
 * necessary for connection to 802.11 Wi-Fi P2P networks (aka Wi-Fi Direct).
 **/

/**
 * NMSettingWifiP2P:
 *
 * Wi-Fi P2P Settings
 *
 * Since: 1.16
 */

/*****************************************************************************/

NM_GOBJECT_PROPERTIES_DEFINE_BASE(PROP_PEER,
                                  PROP_WPS_METHOD,
                                  PROP_WFD_IES,
                                  PROP_WFD_VENDOR_EXTENSIONS,
                                  PROP_WFD_DEVICE_MODE,
                                  PROP_WFD_HOST_NAME,
                                  PROP_WFD_DEVICE_CATEGORY,
                                  PROP_WFD_SECURITY, );

typedef struct {
    char *peer;
    guint32 wps_method;

    /* Wi-Fi Display Connection Setting Properties */
    GBytes *wfd_ies;
    GBytes *wfd_vendor_extensions;
    GBytes *wfd_device_category;
    char   *wfd_device_mode;
    char   *wfd_security_type;
    char   *wfd_host_name;


} NMSettingWifiP2PPrivate;

struct _NMSettingWifiP2P {
    NMSetting               parent;
    NMSettingWifiP2PPrivate _priv;
};

struct _NMSettingWifiP2PClass {
    NMSettingClass parent;
};

G_DEFINE_TYPE(NMSettingWifiP2P, nm_setting_wifi_p2p, NM_TYPE_SETTING)

#define NM_SETTING_WIFI_P2P_GET_PRIVATE(self) \
    _NM_GET_PRIVATE(self, NMSettingWifiP2P, NM_IS_SETTING_WIFI_P2P, NMSetting)

/*****************************************************************************/

/**
 * nm_setting_wifi_p2p_get_peer:
 * @setting: the #NMSettingWifiP2P
 *
 * Returns: the #NMSettingWifiP2P:peer property of the setting
 *
 * Since: 1.16
 **/
const char *
nm_setting_wifi_p2p_get_peer(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting), NULL);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->peer;
}

/**
 * nm_setting_wifi_p2p_get_wps_method:
 * @setting: the #NMSettingWifiP2P
 *
 * Returns: the #NMSettingWifiP2P:wps-method property of the setting
 *
 * Since: 1.16
 **/
NMSettingWirelessSecurityWpsMethod
nm_setting_wifi_p2p_get_wps_method(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting),
                         NM_SETTING_WIRELESS_SECURITY_WPS_METHOD_DEFAULT);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->wps_method;
}

/**
 * nm_setting_wifi_p2p_get_wfd_ies:
 * @setting: the #NMSettingWiFiP2P
 *
 * Returns: (transfer none): the #NMSettingWiFiP2P:wfd-ies property of the setting
 *
 * Since: 1.16
 **/
GBytes *
nm_setting_wifi_p2p_get_wfd_ies(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting), NULL);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->wfd_ies;
}
/**
 * nm_setting_wifi_p2p_get_wfd_device_mode
 * @setting: the #NMSettingWiFiP2P
 * 
 * Returns: the #NMSettingWifiP2P:wfd-device-mode property of the setting
 * 
 * since 1.36
 */
const char *
nm_setting_wifi_p2p_get_wfd_device_mode(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting), NULL);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->wfd_device_mode;
}
/**
 * nm_setting_wifi_p2p_get_vendor_extension_ies:
 * @setting: the #NMSettingWiFiP2P
 *
 * Returns: (transfer none): the #NMSettingWiFiP2P:vendor-extensions property of the setting
 *
 * Since: 1.36
 **/
GBytes *
nm_setting_wifi_p2p_get_vendor_extension_ies(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting), NULL);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->wfd_vendor_extensions;
}
/**
 * nm_setting_wifi_p2p_get_wfd_device_category
 * @setting: the #NMSettingWiFiP2P
 * 
 * Returns: the #NMSettingWifiP2P:wfd-device-category property of the setting
 * 
 * since 1.36
 */
GBytes*
nm_setting_wifi_p2p_get_wfd_device_category(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting), NULL);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->wfd_device_category;
}
/**
 * nm_setting_wifi_p2p_get_wfd_host_name
 * @setting: the #NMSettingWiFiP2P
 * 
 * Returns: the #NMSettingWifiP2P:wfd-host-name property of the setting
 * 
 * since 1.36
 */
const char *
nm_setting_wifi_p2p_get_wfd_host_name(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting), NULL);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->wfd_host_name;
}

/**
 * nm_setting_wifi_p2p_get_wfd_security
 * @setting: the #NMSettingWiFiP2P
 * 
 * Returns: the #NMSettingWifiP2P:wfd-security property of the setting
 * 
 * since 1.36
 */
const char *nm_setting_wifi_p2p_get_wfd_security(NMSettingWifiP2P *setting)
{
    g_return_val_if_fail(NM_IS_SETTING_WIFI_P2P(setting), NULL);

    return NM_SETTING_WIFI_P2P_GET_PRIVATE(setting)->wfd_security_type;
}

/*****************************************************************************/

static gboolean
verify(NMSetting *setting, NMConnection *connection, GError **error)
{
    NMSettingWifiP2PPrivate *priv          = NM_SETTING_WIFI_P2P_GET_PRIVATE(setting);
    const char              *valid_modes[] = {NM_SETTING_WIFI_P2P_MODE_NONE,
                                              NM_SETTING_WIFI_P2P_MODE_SINK,
                                              NM_SETTING_WIFI_P2P_MODE_SOURCE,
                                              NULL};
    const char              *valid_security[] = {NM_SETTING_WIFI_P2P_SECURITY_PUSH_BUTTON,
                                                NM_SETTING_WIFI_P2P_SECURITY_PIN_DISPLAY,
                                                NULL};

    if (priv->wfd_device_mode && !g_strv_contains(valid_modes, priv->wfd_device_mode)) {
        g_set_error(error,
                    NM_CONNECTION_ERROR,
                    NM_CONNECTION_ERROR_INVALID_PROPERTY,
                    _("'%s' is not a valid P2P WFD mode"),
                    priv->wfd_device_mode);
        g_prefix_error(error,
                       "%s.%s: ",
                       NM_SETTING_WIFI_P2P_SETTING_NAME,
                       NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE);
        return FALSE;
    }

    if (priv->wfd_security_type && !g_strv_contains(valid_security, priv->wfd_security_type)) {
        g_set_error(error,
                    NM_CONNECTION_ERROR,
                    NM_CONNECTION_ERROR_INVALID_PROPERTY,
                    _("'%s' is not a valid P2P WFD security type"),
                    priv->wfd_security_type);
        g_prefix_error(error,
                       "%s.%s: ",
                       NM_SETTING_WIFI_P2P_SETTING_NAME,
                       NM_SETTING_WIFI_P2P_WFD_DEVICE_CATEGORY);
        return FALSE;
    }

    // Settings verification that ONLY applies to p2p devices that wish to act as a Miracst Sink
    if (g_strcmp0(priv->wfd_device_mode, NM_SETTING_WIFI_P2P_MODE_SINK) == 0) {
        
        if(!priv->wfd_device_category) {
            g_set_error_literal(error,
                                NM_CONNECTION_ERROR,
                                NM_CONNECTION_ERROR_MISSING_PROPERTY,
                                _("property is missing"));
            g_prefix_error(error,
                           "%s.%s: ",
                           NM_SETTING_WIFI_P2P_SETTING_NAME,
                           NM_SETTING_WIFI_P2P_WFD_DEVICE_CATEGORY);
        }

        if(!priv->wfd_host_name) {
            g_set_error_literal(error,
                                NM_CONNECTION_ERROR,
                                NM_CONNECTION_ERROR_MISSING_PROPERTY,
                                _("property is missing"));
            g_prefix_error(error,
                           "%s.%s: ",
                           NM_SETTING_WIFI_P2P_SETTING_NAME,
                           NM_SETTING_WIFI_P2P_WFD_HOST_NAME);
        }
        
    }
    // Settings verification that is specific to Miracast Sources and Sinks. These settings do not apply to standard p2p connections
    else if (g_strcmp0(priv->wfd_device_mode, NM_SETTING_WIFI_P2P_MODE_NONE) != 0) {
        // Both Miracast Sinks and Sources should both specificy a set of WFD IEs to include in the P2P wifi frames
        if (!priv->wfd_ies) {
            g_set_error_literal(error,
                                NM_CONNECTION_ERROR,
                                NM_CONNECTION_ERROR_MISSING_PROPERTY,
                                _("property is missing"));
            g_prefix_error(error,
                           "%s.%s: ",
                           NM_SETTING_WIFI_P2P_SETTING_NAME,
                           NM_SETTING_WIFI_P2P_WFD_IES);
        }

    }
    // Settings verification that should apply to non-Miracast AND Miracast Sources devices (Source devices behave pretty similarly to a regular p2p device)
    else {
        // Settings verification that is specific to p2p devices that wish to present themselves as a Miracast Source
        if (g_strcmp0(priv->wfd_device_mode, NM_SETTING_WIFI_P2P_MODE_SOURCE) == 0) {
        }
        // Settings verification that is specific to regular p2p devices ONLY
        else {
            if (priv->wfd_ies) {
                g_set_error_literal(
                    error,
                    NM_CONNECTION_ERROR,
                    NM_CONNECTION_ERROR_INVALID_PROPERTY,
                    _("WFD IEs should not be set if the WFD Device Mode is also None"));
                g_prefix_error(error,
                               "%s.%s: ",
                               NM_SETTING_WIFI_P2P_SETTING_NAME,
                               NM_SETTING_WIFI_P2P_WFD_IES);
            }
        }

        // Regular p2p devices, and Miracast sources must specify the peer that they intend to connect with in the settings
        // Miracast Sinks typically start in a state that acts like a "p2p access point" - so there shouldn't be a specified peer that is part of the connection settings
        if (!priv->peer) {
            g_set_error_literal(error,
                                NM_CONNECTION_ERROR,
                                NM_CONNECTION_ERROR_MISSING_PROPERTY,
                                _("property is missing"));
            g_prefix_error(error,
                           "%s.%s: ",
                           NM_SETTING_WIFI_P2P_SETTING_NAME,
                           NM_SETTING_WIFI_P2P_PEER);
            return FALSE;
        }
    }

    // General setting verification that should apply to ALL p2p devices


    if (!_nm_utils_wps_method_validate(priv->wps_method,
                                       NM_SETTING_WIFI_P2P_SETTING_NAME,
                                       NM_SETTING_WIFI_P2P_WPS_METHOD,
                                       TRUE,
                                       error))
        return FALSE;

    return TRUE;
}

/*****************************************************************************/

static void
nm_setting_wifi_p2p_init(NMSettingWifiP2P *setting)
{}

/**
 * nm_setting_wifi_p2p_new:
 *
 * Creates a new #NMSettingWifiP2P object with default values.
 *
 * Returns: (transfer full): the new empty #NMSettingWifiP2P object
 *
 * Since: 1.16
 **/
NMSetting *
nm_setting_wifi_p2p_new(void)
{
    return g_object_new(NM_TYPE_SETTING_WIFI_P2P, NULL);
}

static void
nm_setting_wifi_p2p_class_init(NMSettingWifiP2PClass *setting_wifi_p2p_class)
{
    GObjectClass   *object_class        = G_OBJECT_CLASS(setting_wifi_p2p_class);
    NMSettingClass *setting_class       = NM_SETTING_CLASS(setting_wifi_p2p_class);
    GArray         *properties_override = _nm_sett_info_property_override_create_array();

    object_class->get_property = _nm_setting_property_get_property_direct;
    object_class->set_property = _nm_setting_property_set_property_direct;

    setting_class->verify = verify;

    /**
     * NMSettingWifiP2P:peer:
     *
     * The P2P device that should be connected to. Currently, this is the only
     * way to create or join a group.
     *
     * Since: 1.16
     */
    /* ---keyfile---
     * property: peer
     * format: usual hex-digits-and-colons notation
     * description: MAC address in traditional hex-digits-and-colons notation
     *   (e.g. 00:22:68:12:79:A2), or semicolon separated list of 6 bytes (obsolete)
     *   (e.g. 0;34;104;18;121;162).
     * ---end---
     */
    _nm_setting_property_define_direct_string(properties_override,
                                              obj_properties,
                                              NM_SETTING_WIFI_P2P_PEER,
                                              PROP_PEER,
                                              NM_SETTING_PARAM_NONE,
                                              NMSettingWifiP2P,
                                              _priv.peer);
    
    /**
     * NMSettingWifiP2P:wps-method:
     *
     * Flags indicating which mode of WPS is to be used.
     *
     * There's little point in changing the default setting as NetworkManager will
     * automatically determine the best method to use.
     *
     * Since: 1.16
     */
    _nm_setting_property_define_direct_uint32(properties_override,
                                              obj_properties,
                                              NM_SETTING_WIFI_P2P_WPS_METHOD,
                                              PROP_WPS_METHOD,
                                              0,
                                              G_MAXUINT32,
                                              NM_SETTING_WIRELESS_SECURITY_WPS_METHOD_DEFAULT,
                                              NM_SETTING_PARAM_FUZZY_IGNORE,
                                              NMSettingWifiP2P,
                                              _priv.wps_method);
                                              
    /**
     * NMSettingWifiP2P:wfd-device-mode:
     *
     * These flags indicate what Wi-Fi Display mode the device should function as.
     *
     * Since: 1.36
     */
    _nm_setting_property_define_direct_string(properties_override,
                                              obj_properties,
                                              NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE,
                                              PROP_WFD_DEVICE_MODE,
                                              NM_SETTING_PARAM_NONE,
                                              NMSettingWifiP2P,
                                              _priv.wfd_device_mode);

    /**
     * NMSettingWifiP2P:wfd-host-name:
     *
     * The string used as a device name for Wi-Fi Display capability advertisement.
     *
     * Since: 1.36
     */
    _nm_setting_property_define_direct_string(properties_override,
                                              obj_properties,
                                              NM_SETTING_WIFI_P2P_WFD_HOST_NAME,
                                              PROP_WFD_HOST_NAME,
                                              NM_SETTING_PARAM_NONE,
                                              NMSettingWifiP2P,
                                              _priv.wfd_host_name);
    /**
     * NMSettingWifiP2P:wfd-device-category:
     *
     * The byte string representing a device type WFA code for Wi-Fi Display capability advertisement.
     *
     * Since: 1.36
     */
    _nm_setting_property_define_direct_bytes(properties_override,
                                              obj_properties,
                                              NM_SETTING_WIFI_P2P_WFD_DEVICE_CATEGORY,
                                              PROP_WFD_DEVICE_CATEGORY,
                                              NM_SETTING_PARAM_FUZZY_IGNORE,
                                              NMSettingWifiP2P,
                                              _priv.wfd_device_category);

    /**
     * NMSettingWifiP2P:wfd-security:
     *
     * Security type used for p2p connections
     *
     * Since: 1.36
     */
    _nm_setting_property_define_direct_string(properties_override,
                                                obj_properties,
                                                NM_SETTING_WIFI_P2P_WFD_SECURITY,
                                                PROP_WFD_SECURITY,
                                                NM_SETTING_PARAM_NONE,
                                                NMSettingWifiP2P,
                                                _priv.wfd_security_type);

    /**
     * NMSettingWifiP2P:wfd-vendor-extensions:
     *
     * The vendor extension IEs to include in the p2p beacon
     *
     * Since: 1.36
     */
    _nm_setting_property_define_direct_bytes(properties_override,
                                             obj_properties,
                                             NM_SETTING_WIFI_P2P_WFD_VENDOR_EXTENSIONS,
                                             PROP_WFD_VENDOR_EXTENSIONS,
                                             NM_SETTING_PARAM_FUZZY_IGNORE,
                                             NMSettingWifiP2P,
                                             _priv.wfd_vendor_extensions);

    /**
     * NMSettingWifiP2P:wfd-ies:
     *
     * The Wi-Fi Display (WFD) Information Elements (IEs) to set.
     *
     * Wi-Fi Display requires a protocol specific information element to be
     * set in certain Wi-Fi frames. These can be specified here for the
     * purpose of establishing a connection.
     * This setting is only useful when implementing a Wi-Fi Display client.
     *
     * Since: 1.16
     */
    _nm_setting_property_define_direct_bytes(properties_override,
                                             obj_properties,
                                             NM_SETTING_WIFI_P2P_WFD_IES,
                                             PROP_WFD_IES,
                                             NM_SETTING_PARAM_FUZZY_IGNORE,
                                             NMSettingWifiP2P,
                                             _priv.wfd_ies);

    g_object_class_install_properties(object_class, _PROPERTY_ENUMS_LAST, obj_properties);

    _nm_setting_class_commit(setting_class,
                             NM_META_SETTING_TYPE_WIFI_P2P,
                             NULL,
                             properties_override,
                             0);
}
