# 必要なライブラリをインポート
import cv2
import mediapipe as mp
from pythonosc import udp_client

# MediaPipeのセットアップ
mp_drawing_styles = mp.solutions.drawing_styles
mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

# OSCクライアントのセットアップ
osc_client = udp_client.SimpleUDPClient("127.0.0.1", 50010)   # OSCサーバーのIPアドレスとポートに置き換える

# 利き手を判定する関数の定義
def handedness_judge(multi_hand_landmarks: list, multi_handedness: list):
    handedness = []
    if multi_hand_landmarks is not None and multi_handedness is not None:  # 両方のリストがNoneでないことを確認
        for hand_landmarks, hand_handedness in zip(multi_hand_landmarks, multi_handedness):
            if hand_handedness.classification[0].label == "Left":
                handedness.append("Left")
            elif hand_handedness.classification[0].label == "Right":
                handedness.append("Right")
    return handedness

# OSCを介してランドマーク情報を送信する関数の定義
def send_landmark_info(idx, cx, cy, cz, side):
    landmark_info = f"{side}_Landmark {idx}: ({cx}, {cy}, {cz})"
    print(landmark_info)
    osc_client.send_message("/hand_landmark", landmark_info)

# カメラの入力
cap = cv2.VideoCapture(0)

with mp_hands.Hands(
    model_complexity=0,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as hands:

    while cap.isOpened():
        success, image = cap.read()

        if not success:
            print("Ignoring empty camera frame.")
            continue

        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = hands.process(image)
        multi_handedness = handedness_judge(results.multi_hand_landmarks, results.multi_handedness)

        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        # 両手の座標を保持するリストを初期化
        left_hand_coordinates = [0, 0, 0]
        right_hand_coordinates = [0, 0, 0]

        # ループの前に、multi_hand_landmarksとmulti_handednessがNoneでないことを確認
        if results.multi_hand_landmarks is not None and results.multi_handedness is not None:

            # 各検出された手に対してループ
            for hand_landmarks, side in zip(results.multi_hand_landmarks, multi_handedness):
                for idx, landmark in enumerate(hand_landmarks.landmark):
                    height, width, _ = image.shape
                    cx, cy, cz = int(landmark.x * width), int(landmark.y * height), int(landmark.z * 1000.0)

                     # OSCを介してランドマーク情報を送信
                    if idx == 17:
                        # 特定のランドマーク（idx=17）の座標を抽出
                        if side == "Left":
                            left_hand_coordinates = [cx, cy, cz]
                        elif side == "Right":
                            right_hand_coordinates = [cx, cy, cz]
                    # 画像に手のランドマークを描画
                    mp_drawing.draw_landmarks(
                        image,
                        hand_landmarks,
                        mp_hands.HAND_CONNECTIONS,
                        mp_drawing_styles.get_default_hand_landmarks_style(),
                        mp_drawing_styles.get_default_hand_connections_style())

            # 両手の座標を1つのリストに結合
            combined_coordinates = left_hand_coordinates + right_hand_coordinates

            # 結合された座標をOSC経由で送信
            landmark_info = f"Landmark 17: {combined_coordinates}"
            print(landmark_info)
            osc_client.send_message("/hand_landmark", landmark_info)



        cv2.imshow('MediaPipe Hands', image)

        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
