#include <rtthread.h>
#include <mpu6050_sensor.h>
#include <stdio.h>

int test_mpu6050(int s)
{
	sensors_event_t event;
	MPU6050_Accelerometer acc("i2c1", MPU6050_ADDRESS_AD0_LOW);
	MPU6050_Gyroscope gyro("i2c1", MPU6050_ADDRESS_AD0_LOW);
	acc.activate(RT_TRUE);
	gyro.activate(RT_TRUE);
	while (s-- > 0)
	{
		acc.poll(&event);
		printf("accelerometer - x:%f, y:%f, z:%f\n", 
			event.acceleration.x, event.acceleration.y, event.acceleration.z);
		gyro.poll(&event);
		printf("gyroscope - x:%f, y:%f, z:%f\n", 
			event.gyro.x, event.gyro.y, event.gyro.z);
		rt_thread_delay(10);
	}
	acc.activate(RT_FALSE);
	gyro.activate(RT_FALSE);

	return 0;
}

#ifdef RT_USING_FINSH
#include <finsh.h>

FINSH_FUNCTION_EXPORT(test_mpu6050, --test mpu6050 sensor);

#endif