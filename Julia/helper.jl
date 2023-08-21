ENCODER_LINEAR_ANGULAR_RATIO_CM = 0.6389; # Old: 0.602
ENCODER_LINEAR_ANGULAR_RATIO_M = ENCODER_LINEAR_ANGULAR_RATIO_CM / 100; # Old: 0.00602
ENCODER_ELBOW_ANGULAR_RATIO= 0.2;
ENCODER_SHOULDER_ANGULAR_RATIO = 1.0;
MOTOR_LINEAR_RPM_TO_CMPS = 50; 

decode_list = [
"linear_position", 
"linear_velocity", 
"shoulder_position", 
"shoulder_velocity", 
"elbow_position", 
"elbow_velocity",
"requested_vel_1",
"requested_vel_2",
"serial_tx_time",
# "time_since_last_command",
]

function write_motors(sp, linear_vel, elbow_vel)
    # max prescision of 3 decimal places
    linear_vel = round(linear_vel * MOTOR_LINEAR_RPM_TO_CMPS, digits=3) 
    elbow_vel = round(elbow_vel, digits=3)
    string_to_write = "<" * string(linear_vel) * ", " * string(elbow_vel) * ">"
    write(sp, string_to_write)
end


# Forward declare for runtime optimization
length_decode = length(decode_list)
length_decode_min_1 = length_decode - 1
length_decode_plus_1 = length_decode + 1

function safe_read(sp)
    timeout = 10
    while(timeout > 0)
        encoderData = readline(sp)
        try # catch byte errors in encoderData.decode
            encoderData = split(encoderData, ",")
            encoderData = encoderData[1:length_decode]
            encoderData = [parse(Float64, x) for x in encoderData]
            if length(encoderData) == length_decode # account for the \r\n 
                encoderData[1] *= ENCODER_LINEAR_ANGULAR_RATIO_CM
                encoderData[2] *= ENCODER_LINEAR_ANGULAR_RATIO_CM
                sp_flush(sp, SP_BUF_BOTH)
                return encoderData
            end
            timeout -= 1
            # println("Timeout during Read 1")
        catch
            timeout -= 1
            # println("Timeout during Read 0")
        end
    end
    println("Timeout during Read")
end

function busy_sleep(time_s)
    start = time()
    while (time() - start) < time_s
    end
end