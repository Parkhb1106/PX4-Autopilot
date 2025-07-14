# Global position setpoint triplet in WGS84 coordinates.
# This are the three next waypoints (or just the next two or one).
uint16_t publisher_id

uint64 timestamp		# time since system start (microseconds)

PositionSetpoint previous
PositionSetpoint current
PositionSetpoint next
