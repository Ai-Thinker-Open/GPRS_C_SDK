#ifndef __DEMO_MQTT_H_
#define __DEMO_MQTT_H_


#define BROKER_IP  "mqtt.neucrack.com"
#define BROKER_PORT 8883
#define BROKER_HOSTNAME "mqtt.neucrack.com"
#define CLIENT_ID  "Neucrack1"
#define CLIENT_USER NULL
#define CLIENT_PASS NULL
#define SUBSCRIBE_TOPIC "$neucrack/app"
#define PUBLISH_TOPIC   "$neucrack/gprs"
#define PUBLISH_INTERVAL 10000 //10s
#define PUBLISH_PAYLOEAD "hello I'm from gprs module"


#endif

