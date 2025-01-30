idf_component_register(SRCS "main.c" 
                       "../components/imu_tools/imu_tools.c" 
                       "../components/sensor_imu/sensor_imu.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES imu_tools sensor_imu)
