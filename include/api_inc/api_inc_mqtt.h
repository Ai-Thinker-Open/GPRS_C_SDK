#ifndef _API_INC_MQTT_H
#define _API_INC_MQTT_H


#include "stdint.h"
#include "stdbool.h"

typedef enum{
    MQTT_ERROR_NONE       =  0,  //no error
    /** Out of memory error.     */
    MQTT_ERROR_MEM        = -1,
    /** Buffer error.            */
    MQTT_ERROR_BUF        = -2,
    /** Timeout.                 */
    MQTT_ERROR_TIMEOUT    = -3,
    /** Routing problem.         */
    MQTT_ERROR_RTE        = -4,
    /** Operation in progress    */
    MQTT_ERROR_INPROGRESS = -5,
    /** Illegal value.           */
    MQTT_ERROR_VAL        = -6,
    /** Operation would block.   */
    MQTT_ERROR_WOULDBLOCK = -7,
    /** Address in use.          */
    MQTT_ERROR_USE        = -8,
    /** Already connecting.      */
    MQTT_ERROR_ALREADY    = -9,
    /** Conn already established.*/
    MQTT_ERROR_ISCONN     = -10,
    /** Not connected.           */
    MQTT_ERROR_CONN       = -11,
    /** Low-level netif error    */
    MQTT_ERROR_IF         = -12,

    /** Connection aborted.      */
    MQTT_ERROR_ABRT       = -13,
    /** Connection reset.        */
    MQTT_ERROR_RST        = -14,
    /** Connection closed.       */
    MQTT_ERROR_CLSD       = -15,
    /** Illegal argument.        */
    MQTT_ERROR_ARG        = -16,
    MQTT_ERROR_PARAM      = -17,
    MQTT_ERROR_DNS        = -18
}MQTT_Error_t;

typedef struct{
    void* mqttClient;
}MQTT_Client_t;

typedef enum{
    MQTT_SSL_VERSION_SSLv3 = 0,
    MQTT_SSL_VERSION_TLSv1    ,
    MQTT_SSL_VERSION_TLSv1_1  ,
    MQTT_SSL_VERSION_TLSv1_2  ,
    MQTT_SSL_VERSION_MAX
}MQTT_SSL_Version_t;

typedef enum{
    MQTT_SSL_VERIFY_MODE_NONE     = 0,
    MQTT_SSL_VERIFY_MODE_OPTIONAL = 1,
    MQTT_SSL_VERIFY_MODE_REQUIRED = 2,
    MQTT_SSL_VERIFY_MODE_MAX
}MQTT_SSL_Verify_Mode_t;

typedef struct{
    /** Client identifier, must be set by caller  */
    const char *client_id;
    /** User name, set to NULL if not used */
    const char* client_user;
    /** Password, set to NULL if not used */
    const char* client_pass;
    /** keep alive time in seconds, 0 to disable keep alive functionality*/
    uint16_t    keep_alive;
    /** will topic, set to NULL if will is not to be used,
         will_msg, will_qos and will retain are then ignored */
    const char* will_topic;
    /** will_msg, see will_topic */
    const char* will_msg;
    /** will_qos, see will_topic */
    uint8_t     will_qos;
    /** will_retain, see will_topic */
    uint8_t     will_retain;
    uint8_t     clean_session;
    /** SSL/TLS configuration for secure connections */
    bool                   use_ssl;
    MQTT_SSL_Verify_Mode_t ssl_verify_mode;
    const char*            ca_cert;
    const char*            ca_crl;
    const char*            client_cert;
    const char*            client_key;
    const char*            client_key_passwd;
    const char*            broker_hostname;
    MQTT_SSL_Version_t     ssl_min_version;
    MQTT_SSL_Version_t     ssl_max_version;
    const char*            entropy_custom;
}MQTT_Connect_Info_t;

typedef enum
{
    /** Accepted */
    MQTT_CONNECTION_ACCEPTED                 = 0,
    /** Refused protocol version */
    MQTT_CONNECTION_REFUSED_PROTOCOL_VERSION = 1,
    /** Refused identifier */
    MQTT_CONNECTION_REFUSED_IDENTIFIER       = 2,
    /** Refused server */
    MQTT_CONNECTION_REFUSED_SERVER           = 3,
    /** Refused user credentials */
    MQTT_CONNECTION_REFUSED_USERNAME_PASS    = 4,
    /** Refused not authorized */
    MQTT_CONNECTION_REFUSED_NOT_AUTHORIZED   = 5,
    /** Disconnected */
    MQTT_CONNECTION_DISCONNECTED             = 256,
    /** Timeout */
    MQTT_CONNECTION_TIMEOUT                  = 257,
    MQTT_CONNECTION_DNS_FAIL                 = 258,
    MQTT_CONNECTION_STATUS_MAX
}MQTT_Connection_Status_t;

typedef enum{
    MQTT_FLAG_NONE      = 0   ,
    MQTT_FLAG_DATA_LAST = 1 ,
    MQTT_FLAG_MAX
}MQTT_Flags_t;

// typedef enum {
//     MQTT_SSL_MD_NONE=0,
//     MQTT_SSL_MD_MD2,
//     MQTT_SSL_MD_MD4,
//     MQTT_SSL_MD_MD5,
//     MQTT_SSL_MD_SHA1,
//     MQTT_SSL_MD_SHA224,
//     MQTT_SSL_MD_SHA256,
//     MQTT_SSL_MD_SHA384,
//     MQTT_SSL_MD_SHA512,
//     MQTT_SSL_MD_RIPEMD160,
// }MQTT_SSL_MD_t;

typedef void (*MQTT_Connection_Callback_t)(MQTT_Client_t* client, void* arg, MQTT_Connection_Status_t status);
typedef void (*MQTT_Request_Callback_t)(void* arg, MQTT_Error_t err);
typedef void (*MQTT_InPub_Callback_t)(void* arg, const char* topic, uint32_t payloadLen);
typedef void (*MQTT_InPub_Data_Callback_t)(void* arg, const uint8_t* data, uint16_t len, MQTT_Flags_t flags);


#endif
