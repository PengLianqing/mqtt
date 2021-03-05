#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "MQTTClient.h"

#define ADDRESS     "211.67.16.19"
#define CLIENTID    "cpp-mqtt-0001"
#define TOPIC       "/python/mqtt"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
  
    // Connection parameters
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    static int msg_count = 0;
    char publish_message_buf[128];
    while (msg_count < 100) {
      sprintf(publish_message_buf,
              "{\"messages\":%d,\"userData1\":%d,\"userData2\":%4.2f}",
              msg_count++, rand() % 128, rand() % 1024 * 0.01f);
      // Publish message
      pubmsg.payload = publish_message_buf;
      pubmsg.payloadlen = strlen(publish_message_buf);
      pubmsg.qos = QOS;
      pubmsg.retained = 0;
      MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
      // printf("Waiting for up to %d seconds for publication of %s\n"
      //     "on topic %s for client with ClientID: %s\n",
      //     (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
      rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
      printf("Message with delivery token %d delivered\n", token);
    }

    // Disconnect
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}