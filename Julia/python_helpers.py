import time


def busy_sleep(duration, get_now=time.perf_counter):
    now = get_now()
    end = now + duration
    while now < end:
        now = get_now()

def safe_read(ser):
    timeout = 10
    while(timeout > 0):
        encoderData = ser.readline()
        try: # catch byte errors in encoderData.decode
            encoderData = encoderData.decode()
            encoderData = encoderData.split(",")
            if len(encoderData) - 1 == len(decode_list): # account for the \r\n 
                return encoderData[:-1]
            timeout -= 1
        except:
            timeout -= 1
    raise Exception("Timeout during Read")

def safe_write(ser, motor_1_speed, motor_2_speed):
    motor_1_speed = float(motor_1_speed, 3)
    motor_2_speed = float(motor_2_speed, 3)
    command = "<" + str(motor_1_speed) + "," + str(motor_2_speed) + ">"

    timeout = 10
    while(timeout > 0):
        try:
            ser.write(data)
            return
        except:
            timeout -= 1
    raise Exception("Timeout during Write")