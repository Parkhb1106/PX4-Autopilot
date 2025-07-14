#
# Pre-flight sensor check metrics.
# The topic will not be updated when the vehicle is armed
#
uint64 timestamp # time since system start (microseconds)
uint16_t publisher_id

float32 mag_inconsistency_angle # maximum angle between magnetometer instance field vectors in radians.
