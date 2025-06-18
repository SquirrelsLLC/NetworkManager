/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019 Red Hat, Inc.
 */

#ifndef __NM_SETTING_WIFI_P2P_H__
#define __NM_SETTING_WIFI_P2P_H__

#if !defined(__NETWORKMANAGER_H_INSIDE__) && !defined(NETWORKMANAGER_COMPILATION)
#error "Only <NetworkManager.h> can be included directly."
#endif

#include "nm-setting.h"
#include "nm-setting-wireless-security.h"

G_BEGIN_DECLS

#define NM_TYPE_SETTING_WIFI_P2P (nm_setting_wifi_p2p_get_type())
#define NM_SETTING_WIFI_P2P(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), NM_TYPE_SETTING_WIFI_P2P, NMSettingWifiP2P))
#define NM_SETTING_WIFI_P2P_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), NM_TYPE_SETTING_WIFI_P2P, NMSettingWifiP2PClass))
#define NM_IS_SETTING_WIFI_P2P(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), NM_TYPE_SETTING_WIFI_P2P))
#define NM_IS_SETTING_WIFI_P2P_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), NM_TYPE_SETTING_WIFI_P2P))
#define NM_SETTING_WIFI_P2P_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS((obj), NM_TYPE_SETTING_WIFI_P2P, NMSettingWifiP2PClass))

#define NM_SETTING_WIFI_P2P_SETTING_NAME "wifi-p2p"

/**
 * NM_SETTING_WIFI_P2P_PEER:
 *
 * The mac address of the peer to connect to.
 */
#define NM_SETTING_WIFI_P2P_PEER                "peer"

#define NM_SETTING_WIFI_P2P_WPS_METHOD          "wps-method"
/**
 * NM_SETTING_WIFI_P2P_WFD_IES:
 *
 * The Wi-Fi Dispaly Information Elements to use
 */
#define NM_SETTING_WIFI_P2P_WFD_IES             "wfd-ies"
/**
 * NM_SETTING_WIFI_P2P_WFD_VENDOR_EXTENSIONS:
 *
 * The vendor extension IEs to include in the p2p beacon
 */
#define NM_SETTING_WIFI_P2P_WFD_VENDOR_EXTENSIONS "wfd-vendor-extensions"
/**
 * NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE:
 *
 * The Wi-Fi Display Device mode to operate in.
 */
#define NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE     "wfd-device-mode"
/**
 * NM_SETTING_WIFI_P2P_WFD_GO_INTENT
 * 
 * The value to use (1-14) when negotiating which p2p device in a group should be the GO
 */
#define NM_SETTING_WIFI_P2P_WFD_GO_INTENT      "wfd-go-intent"
/**
 * NM-SETTING-WIFI-P2P-WFD-PERSISTENT-RECONNECT
 * 
 * Whether persistent reconnect should be supported or not when making p2p connections
 */
#define NM_SETTING_WIFI_P2P_WFD_PERSISTENT_RECONNECT "wfd-persistent-reconnect"
/**
 * NM_SETTING_WIFI_P2P_WFD_DEVICE_NAME:
 *
 * The Wi-Fi Display device name to advertise.
 */
#define NM_SETTING_WIFI_P2P_WFD_DEVICE_NAME       "wfd-device-name"
/**
 * NM_SETTING_WIFI_P2P_WFD_DEVICE_CATEGORY:
 *
 * The Wi-Fi Alliance WFA Device Type Code to use in the discovery beacon
 */
#define NM_SETTING_WIFI_P2P_WFD_DEVICE_CATEGORY "wfd-device-category"
/**
 * NM_SETTING_WIFI_P2P_WFD_SECURITY:
 *
 * The type of connection security used when making a P2P connection
 */
#define NM_SETTING_WIFI_P2P_WFD_SECURITY "wfd-security"

/**
 * NM_SETTING_WIFI_P2P_WFD_LISTEN_ONLY:
 *
 * Wether we should use p2p-find (false) or p2p-listen (true)
 */
#define NM_SETTING_WIFI_P2P_WFD_LISTEN_ONLY "wfd-listen-only"

/**
 * NM_SETTING_WIFI_P2P_WFD_LISTEN_PERIOD:
 *
 * The timeout used when calling p2p-find or p2p-listen
 */
#define NM_SETTING_WIFI_P2P_WFD_LISTEN_PERIOD "wfd-listen-period"

/**
 * NM_SETTING_WIFI_P2P_WFD_LISTEN_INTERVAL:
 *
 * The timeout used to make repeated p2p-find or p2p-listen calls after the initial call is made
 */
#define NM_SETTING_WIFI_P2P_WFD_LISTEN_INTERVAL "wfd-listen-interval"


/**
 * NM_SETTING_WIFI_P2P_MODE_NONE:
 * 
 * Indicates that the conneciton is not concerned Wi-Fi Display / Miracast
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_MODE_NONE "none"
/**
 * NM_SETTING_WIFI_P2P_MODE_SOURCE:
 * 
 * Indicates that the conneciton is concerned with Wi-Fi Display / Miracast
 * and wants to operate as a Miracast Source device
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_MODE_SOURCE "wfd-source"
/**
 * NM_SETTING_WIFI_P2P_MODE_SINK:
 * 
 * Indicates that the conneciton is concerned with Wi-Fi Display / Miracast
 * and wants to operate as a Miracast Sink device
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_MODE_SINK "wfd-sink"

/**
 * NM_SETTING_WIFI_P2P_SECURITY_PBC:
 * 
 * PBC security
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_SECURITY_PUSH_BUTTON "pbc"
/**
 * NM_SETTING_WIFI_P2P_SECURITY_PIN:
 * 
 * PIN security
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_SECURITY_PIN_DISPLAY "pin"



typedef struct _NMSettingWifiP2PClass NMSettingWifiP2PClass;

NM_AVAILABLE_IN_1_16
GType nm_setting_wifi_p2p_get_type(void);

NM_AVAILABLE_IN_1_16
NMSetting *nm_setting_wifi_p2p_new(void);

NM_AVAILABLE_IN_1_16
const char *nm_setting_wifi_p2p_get_peer(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_16
NMSettingWirelessSecurityWpsMethod nm_setting_wifi_p2p_get_wps_method(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_16
GBytes *nm_setting_wifi_p2p_get_wfd_ies(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
GBytes *nm_setting_wifi_p2p_get_wfd_device_category(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
const char *nm_setting_wifi_p2p_get_wfd_device_mode(NMSettingWifiP2P * setting);

NM_AVAILABLE_IN_1_36
GBytes *nm_setting_wifi_p2p_get_vendor_extension_ies(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
gint nm_setting_wifi_p2p_get_wfd_go_intent(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
gboolean nm_setting_wifi_p2p_get_wfd_persistent_reconnect(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
const char *nm_setting_wifi_p2p_get_wfd_device_name(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
const char *nm_setting_wifi_p2p_get_wfd_security(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
gboolean nm_setting_wifi_p2p_get_wfd_listen_only(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
gint nm_setting_wifi_p2p_get_wfd_listen_period(NMSettingWifiP2P *setting);

NM_AVAILABLE_IN_1_36
gint nm_setting_wifi_p2p_get_wfd_listen_interval(NMSettingWifiP2P *setting);

G_END_DECLS

#endif /* __NM_SETTING_WIFI_P2P_H__ */
