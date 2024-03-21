import cv2
from deepface import DeepFace
from selenium import webdriver
from time import sleep

cap = cv2.VideoCapture(0, cv2.CAP_V4L2)

cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

counter = 0
face_match = False
choice = 0

reference_img_shobhit = cv2.imread(
    "/home/shobhit/Downloads/Edge/projects/Electronic-Vault/faceAuthentication/reference_shobhit.jpg"
)
reference_img_prachi = cv2.imread(
    "/home/shobhit/Downloads/Edge/projects/Electronic-Vault/faceAuthentication/reference_prachi.jpg"
)


def check_face(frame):
    global face_match
    global choice
    try:
        result = 0
        if choice == 0:
            result = DeepFace.verify(frame, reference_img_shobhit.copy())
            choice = 1
        else:
            result = DeepFace.verify(frame, reference_img_prachi.copy())
            choice = 0
        if result["verified"]:
            face_match = True
        else:
            face_match = False
    except ValueError as e:
        print(f"Error during face verification: {str(e)}")
        face_match = False


while True:
    ret, frame = cap.read()

    if ret:
        if counter % 30 == 0:
            check_face(frame.copy())
        counter += 1
        if face_match:
            cv2.destroyAllWindows()
            cap.release()

            url = "https://wokwi.com/projects/387337896026447873"
            browser = webdriver.Firefox()
            browser.get(url)
            sleep(1800)  # 30 mins
            browser.quit()
        else:
            cv2.putText(
                frame,
                "NO MATCH!",
                (20, 450),
                cv2.FONT_HERSHEY_SIMPLEX,
                2,
                (0, 0, 255),
                3,
            )

        cv2.imshow("video", frame)

    key = cv2.waitKey(1)
    if key == ord("q"):
        break
