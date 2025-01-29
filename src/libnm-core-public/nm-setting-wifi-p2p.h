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
#define NM_SETTING_WIFI_P2P_WFD_IES             "wfd-ies"
#define NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE     "wfd-device-mode"


/**
 * NM_SETTING_WIFI_P2P_MODE_NONDE
 * 
 * Indicates that the conneciton is not concerned Wi-Fi Display / Miracast
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_MODE_NONE "none"
/**
 * NM_SETTING_WIFI_P2P_MODE_SOURCE
 * 
 * Indicates that the conneciton is concerned with Wi-Fi Display / Miracast
 * and wants to operate as a Miracast Source device
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_MODE_SOURCE "wfd-source"
/**
 * NM_SETTING_WIFI_P2P_MODE_SINK
 * 
 * Indicates that the conneciton is concerned with Wi-Fi Display / Miracast
 * and wants to operate as a Miracast Sink device
 * 
 * Since: 1.36
 */
#define NM_SETTING_WIFI_P2P_MODE_SINK "wfd-sink"

/**
 * NMSettingWifiP2pWfdDeviceMode
 * @NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE_NONE: use the default value
 * @NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE_SOURCE: don't touch existing setting
 * @NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE_SINK: disable powersave
 *
 * These flags indicate what Wi-Fi Display mode the device should function as.
 * 
 * since 1.36
 **/
typedef enum {                                              
    NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE_NONE        = 0,    
    NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE_SOURCE      = 1,
    NM_SETTING_WIFI_P2P_WFD_DEVICE_MODE_SINK        = 2,
} NMSettingWifiP2pWfdDeviceMode;



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
const char *nm_setting_wifi_p2p_get_wfd_device_mode(NMSettingWifiP2P * setting);

G_END_DECLS

#endif /* __NM_SETTING_WIFI_P2P_H__ */
