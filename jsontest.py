#!/usr/bin/env python
# coding=utf-8
import json

#JSON字符串操作
jsonData = '''[
    {"a":1,"b":2,"c":3,"d":4,"e":5},
    {"a":6,"b":7,"c":8,"d":9,"e":10}
]'''
# jsonData = '[{"a":1,"b":2,"c":3,"d":4,"e":5},{"a":6,"b":7,"c":8,"d":9,"e":10}]'
text = json.loads(jsonData) #对数据进行解码,将JSON对象解码为Python字典
print(text) #字典
print(text[0]) #JSON数组中的一个对象
print(text[1]['a']) #JSON数组中个对象的元素
json_str = json.dumps(text) #对数据进行编码,将Python字典编码为JSON对象
print(json_str)

# JSON文件操作
# 读取数据
with open('read_test.json', 'r') as f:
    data=json.load(f)
# 写入 JSON 数据
with open('write_test.json', 'w') as f:
    json.dump(data, f)