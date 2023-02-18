/*
 * mqtt_config.h
 *
 *  Created on: Feb 14, 2023
 *      Author: khanh
 */

#ifndef APP_CONFIG_MQTT_CONFIG_H_
#define APP_CONFIG_MQTT_CONFIG_H_

#define MQTT_CONFIG_QOS					"2"
#define MQTT_CONFIG_RETAIN				"0"

#define MQTT_NETWORK_HOST				"mqtt.smiletech.vn"
#define MQTT_NETWORK_PORT				"8885"
#define MQTT_CLIENT_USERNAME			"tpms_device"
#define MQTT_CLIENT_PASSWORD			"2hwkK7NVfLyHtPx"

#define MQTT_TOPIC_ROOT_PATH			"/device/"
#define MQTT_TOPIC_LOCATION_ENDPOINT	"/location"
#define MQTT_TOPIC_SENSOR_ENDPOINT		"/sensor"


#endif /* APP_CONFIG_MQTT_CONFIG_H_ */
