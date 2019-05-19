# simpelLidar
////////////////////////////////////////////
传感器
※编码器：
	160 ppr(相对于轮子)
※驱动板：
	12V电压输出，控制电路电源：3V3 
※Lidar
	型号：HLS-LFCD2
	有效范围：120mm~3500mm
	采样范围：1.8kHz
	支持电压：5V DC±5%（注意共地，没法共电源，不支持3V3）
	UART波特率：230400bps
	电流范围：400ms（峰值电流1A）
	Lidar速度：增加PWM控制Lidar扫描速度控制
注意:Lidar模块的PWM引脚无需控制，供Lidar模块使用，只需要连接电源，UART的Tx和Rx，进行数据读取即可。
※姿态模块
	MPU9250

////////////////////////////////////////////
底盘平台
※120度全向轮平台
※马达：80:1减速比输出

小车底盘接线图：
※Motor 1:
	IN_l1	IN_r1	EN1
	PIN8	PIN9	PIN7
※Motor 2:
	IN_l2	IN_r2	EN2
	PIN0	PIN2	PIN3
※Motor 3:
	IN_l3	IN_r3	EN3
	PIN12	PIN13	PIN14

////////////////////////////////////////////
注意事项：
1.小车马达驱动板的供电要用H5主板的3v3,GND.（不能与7805共地，取5V电源,控制信号电压要与电源进行匹配！否则无法驱动马达。
2.目前使用的测速编码器使用5V电源板供电，其输出的信号应该是5V，直接连在H5芯片管脚，作为输入信号，存在信号电压不兼容的隐患。
3.突然想到之前测试UART发送数据到Lidar，Lidar无响应问题，估计也是电源的问题，现在所有的有外设电源都要与主板同3V3？共地也不行吗？
	-->Ldiar UART支持3V3信号，不存在此问题。
4.目前Lidar测试情况是100ms poll一次数据，可以正常得结果。时间过长或者过短都会出现问题。不确定是最大采样率，有待以后改进。
