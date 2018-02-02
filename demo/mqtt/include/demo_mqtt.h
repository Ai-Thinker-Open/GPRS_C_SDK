#ifndef __DEMO_MQTT_H_
#define __DEMO_MQTT_H_


#define BROKER_IP  "mqtt.neucrack.com"
#define BROKER_PORT 1883
#define CLIENT_ID  "Neucrack"
#define CLIENT_USER "mqtt"
#define CLIENT_PASS "mqtt"
#define SUBSCRIBE_TOPIC "$neucrack/app"
#define PUBLISH_TOPIC   "$neucrack/gprs"
#define PUBLISH_INTERVAL 10000 //10s
#define PUBLISH_PAYLOEAD "hello I'm from gprs module"


#endif

