#ifndef __SLAM_H__
#define __SLAM_H__

extern int slam_uart_init(void);
extern int slam_speed_adj(unsigned char speed);
extern void slam_start(void);
extern void slam_srop(void);
extern void slam_lidar_poll(void);

#endif