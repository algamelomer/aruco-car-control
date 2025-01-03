import cv2
import requests
import numpy as np
# http://192.168.0.102/
esp32_ip = "192.168.0.115"  # Set your ESP32-CAM IP
url = f"http://{esp32_ip}/"

def stream():
    try:
        # Get the image from the ESP32-CAM
        response = requests.get(url, stream=True, timeout=1)
        response.raise_for_status()  # Raise an error for bad responses

        # Read the image data
        bytes_data = bytes()
        img = None
        
        for chunk in response.iter_content(chunk_size=1024):
            bytes_data += chunk
            # Check for JPEG header and footer
            a = bytes_data.find(b'\xff\xd8')  # JPEG Start
            b = bytes_data.find(b'\xff\xd9')  # JPEG End
            
            if a != -1 and b != -1:
                jpg = bytes_data[a:b + 2]  # Extract JPEG image
                bytes_data = bytes_data[b + 2:]  # Remove processed data
                img = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
                break  # Break to display each frame once processed

        return img
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")
        return None

if __name__ == "__main__":
    cv2.namedWindow('ESP32-CAM Stream', cv2.WINDOW_NORMAL)  # Create a window
    while True:
        img = stream()

        if img is not None:  # Check if img is valid
            cv2.imshow('ESP32-CAM Stream', img)
        else:
            print("Failed to retrieve image.")

        # Exit on 'q' press
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()
