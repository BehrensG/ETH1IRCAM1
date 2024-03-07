import cv2
import numpy as np
import socket
from PIL import Image

UDP_IP = "192.168.1.56"  # IP address of the UDP server
UDP_PORT = 16384       # Port number the UDP server is listening on



width = 160
height = 120
# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
recevied_blocks = 0
rest_of_data = b""
ack_count = 0
# Bind the socket to the server address and port
#sock.bind((UDP_IP, UDP_PORT))

sock.sendto("start".encode(), (UDP_IP,UDP_PORT))

# Initialize video capture
cap = cv2.VideoCapture(0)  # You can also specify a video file path here

while True:
    rest_of_data =b""
    recevied_blocks = 0
    block_count = 1
    # Receive data from UDP socket
    while recevied_blocks < 4:
        # Receive data from the server
        data, addr = sock.recvfrom(14428)  # buffer size is 1024 bytes
        if data[20] == block_count :
            rest_of_data += data[28:]
            recevied_blocks +=1
            block_count +=1
    
    ack_count +=1
    if ack_count == 60 :
        sock.sendto("ack".encode(), (UDP_IP,UDP_PORT))
        ack_count = 0


    # Decode the received data into a NumPy array
    # Reshape the NumPy array to the original frame shape
    image_array = np.frombuffer(rest_of_data, dtype=np.uint8)
    image_array = image_array.reshape((height, width,3))
    rescaled_image = cv2.resize(image_array, (width*4, height*4))
    # Display the frameq
    cv2.imshow('Video Stream', rescaled_image)
    
    # Check for exit key
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release resources
  
sock.sendto("stop".encode(), (UDP_IP,UDP_PORT))
cap.release()
cv2.destroyAllWindows()
sock.close()

