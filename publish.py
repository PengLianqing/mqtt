#!/usr/bin/env python
# coding=utf-8
# publish端
from paho.mqtt import client as mqtt_client
import time
import random

host = "211.67.16.19"
port = 1883 # 1883是MQTT:TCP协议部署EMQX用到的端口。
topic = "/python/mqtt"
client_id = 'python-mqtt-0001'

# 回调函数。当尝试与 MQTT broker 建立连接时，触发该函数。
# client 是本次连接的客户端实例。
# userdata 是用户的信息，一般为空。但如果有需要，也可以通过 user_data_set 函数设置。
# flags 保存服务器响应标志的字典。
# rc 是响应码:
# 0: 连接成功
# 1: 连接失败-不正确的协议版本
# 2: 连接失败-无效的客户端标识符
# 3: 连接失败-服务器不可用
# 4: 连接失败-错误的用户名或密码
# 5: 连接失败-未授权
def connect_mqtt(): #mqtt连接函数
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
    
    client = mqtt_client.Client(client_id)
    client.on_connect = on_connect
    
    # 设置遗嘱消息，当异常关闭，或者网络出现异常中断时，发送遗嘱消息给其他客户端
    client.will_set('python/status',  b'{"status": "Off"}')

    # 创建连接，三个参数分别为 broker 地址，broker 端口号，保活时间
    client.connect(host, port)
    return client

def publish(client): #发布消息
     msg_count = 0
     while True:
         time.sleep(0.001)
         msg = f"messages: {msg_count},{random.randint(0,128)}"
         #四个参数分别为：主题，发送内容，QoS, 是否保留消息
         #client.publish('raspberry/topic', payload=i, qos=0, retain=False)
         result = client.publish(topic, msg)
         # result: [0, 1]
         status = result[0]
         if status == 0:
             print(f"Send `{msg}` to topic `{topic}`")
         else:
             print(f"Failed to send message to topic {topic}")
         msg_count += 1

def subscribe(client: mqtt_client): #订阅消息
    def on_message(client, userdata, msg):
        print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

    client.subscribe(topic)
    client.on_message = on_message

def run():
    client = connect_mqtt() #连接mqtt
    #subscribe(client) #订阅消息
    client.loop_start() #开启网络循环
    publish(client) #发布消息
    # 设置网络循环堵塞，在调用 disconnect() 或程序崩溃前，不会主动结束程序
    client.loop_forever()

if __name__ == '__main__':
    run()