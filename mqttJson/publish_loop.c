#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "time.h"
#include "MQTTClient.h"

#define ADDRESS     "211.67.16.19"
#define CLIENTID    "cpp-mqtt-0001"
#define TOPIC       "/python/mqtt"
// QoS0，At most once，至多一次；
// QoS1，At least once，至少一次；
// QoS2，Exactly once，确保只有一次。
#define QOS         0
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{
    // 创建mqtt客户端
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    srand(time(NULL));
    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
  
    // 连接mqtt服务器
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
        usleep(10000);
        sprintf(publish_message_buf,
                "{\"messages\":%d,\"userDate\":%d,\"userStr\":%4.2f}",
                msg_count++, rand() % 128, rand() % 1024 * 0.01f);
        // 发布消息
        pubmsg.payload = publish_message_buf;
        pubmsg.payloadlen = strlen(publish_message_buf);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        // printf("Waiting for up to %d seconds for publication of %s\n"
        //     "on topic %s for client with ClientID: %s\n",
        //     (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        //printf("Message with delivery token %d delivered\n", token);
    }

    // 断开服务器连接
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}