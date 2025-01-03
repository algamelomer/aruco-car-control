from test_2 import *
import requests
import numpy as np
import time

esp32_ip = "http://192.168.0.115"

def run_aruco_marker_pose_estimation(aruco_type): 
    aruco_marker = ArucoMarkers() 
    camera_matrix = np.array([
        [1432.0, 0.0, 983.0], 
        [0.0, 1434.0, 561.0], 
        [0.0, 0.0, 1.0]
    ])  
    dist_coeffs = np.array([0.05994318, -0.26432366, -0.00135378, -0.00081574, 0.29707202])

    command_sent_time = 0  # Track the last time a command was sent
    cooldown = 3  # Cooldown period in seconds

    for data in aruco_marker.aruco_marker_pose_estimation(aruco_type, camera_matrix, dist_coeffs):
        distance = data[0]
        ids = data[1]
        position = data[2]
        print(f"Distance: {distance:.2f} meters")
        print(f"position: {position}")

        # print(id)
        # Check if distance condition is met and cooldown has passed
        for id in ids:
            if id == 20:
                if distance <=12  and (time.monotonic() - command_sent_time >= cooldown):
                    command = "park"
                    command_url = f"{esp32_ip}:81/cmd?cmd={command}"
                    try:
                        response = requests.get(command_url)
                        print(f"Command sent: {command}, Response: {response.text}")
                        command_sent_time = time.monotonic()  # Reset the timer
                
                    except requests.RequestException as e:
                        print(f"Failed to send command {command}: {e}")
            
            if id == 22:
                if distance >10:
                    command = "forward"
                    command_url = f"{esp32_ip}:81/cmd?cmd={command}"
                    try:
                        response = requests.get(command_url)
                        print(f"Command sent: {command}, Response: {response.text}")
                        command_sent_time = time.monotonic()  # Reset the timer
                
                    except requests.RequestException as e:
                        print(f"Failed to send command {command}: {e}")



        # if distance <= 15 and (time.monotonic() - command_sent_time >= cooldown):
        #     command = "turn_right"
        #     command_url = f"{esp32_ip}:81/cmd?cmd={command}"
            
        #     try:
        #         response = requests.get(command_url)
        #         print(f"Command sent: {command}, Response: {response.text}")
        #         command_sent_time = time.monotonic()  # Reset the timer
                
        #     except requests.RequestException as e:
        #         print(f"Failed to send command {command}: {e}")

if __name__ == "__main__":
    run_aruco_marker_pose_estimation(ArucoType.DICT_6X6_250)
