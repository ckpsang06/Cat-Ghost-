# Cat-Ghost Game

![Gameplay Banner](https://github.com/ckpsang06/Cat-Ghost-/blob/main/res/images/menu1_2.png)

## 1. Mô tả ngắn gọn bài toán và phạm vi hệ thống
**Cat-Ghost** là một tựa game sinh tồn 2D góc nhìn ngang, trong đó người chơi điều khiển một chú mèo pháp sư chiến đấu chống lại những bóng ma liên tục xuất hiện. Mục tiêu của trò chơi là sinh tồn và tiêu diệt ma để tích lũy điểm số (`SCORE`) trong một khoảng thời gian giới hạn là 500 giây (500000 ms). 

**Hệ thống bao gồm các màn hình chính:** 
*   **Menu khởi đầu:**
  
![Gameplay Banner](https://github.com/ckpsang06/Cat-Ghost-/blob/main/res/images/menu1_2.png)

*   **Màn hình chơi chính:**

![Menu](https://github.com/ckpsang06/Cat-Ghost-/blob/group-3/Screenshot%202026-08-20%20215051.png)

*   **Màn hình kết thúc (Thắng/Thua):**

![Menu](https://github.com/ckpsang06/Cat-Ghost-/blob/group-3/Screenshot%202026-08-20%20215122.png)
![Menu](https://github.com/ckpsang06/Cat-Ghost-/blob/group-3/Screenshot%202026-08-20%20215331.png)

Trò chơi kết hợp cơ chế chiến đấu cơ bản và hệ thống chiêu thức ma thuật có thời gian chờ (cooldown).

## 2. Video Demo & Báo cáo

**Video Demo Gameplay:**

https://drive.google.com/file/d/16yhC8b9zXx8WD_ZHuGO2zHU2f26KydwF/view?usp=sharing

**Báo cáo:**

https://drive.google.com/file/d/1GOMGoR0I8ifrYd_cPGGacF7glzmTb5Wi/view?usp=sharing

## 3. Công nghệ sử dụng, môi trường chạy và yêu cầu cài đặt
*   **Ngôn ngữ lập trình:** C++.
*   **Thư viện đồ họa & âm thanh:** 
    *   `SDL2` (Xử lý cửa sổ và đồ họa cơ bản).
    *   `SDL2_image` (Tải hình ảnh định dạng PNG/JPG).
    *   `SDL2_ttf` (Xử lý font chữ).
    *   `SDL2_mixer` (Xử lý âm thanh, nhạc nền).
*   **Môi trường chạy:** Máy tính hệ điều hành Windows/Linux/macOS có hỗ trợ trình biên dịch C++ (như GCC/MinGW).
*   **Yêu cầu cài đặt:** Cần cài đặt đầy đủ bộ thư viện SDL2 và cấu hình biến môi trường (hoặc thiết lập Linker trong IDE như Code::Blocks/Visual Studio) trỏ đến các file `.dll` và `.a` / `.lib` tương ứng trước khi biên dịch.

## 4. Cấu trúc thư mục hoặc các module chính
Mã nguồn được chia thành các module hướng đối tượng rõ ràng:
*   **`main.cpp`**: Điểm khởi chạy của chương trình, chịu trách nhiệm khởi tạo game và duy trì vòng lặp trò chơi (Game Loop) ở mức 60 FPS.
*   **`CommonFunc.h`**: Chứa các hằng số dùng chung toàn cục như kích thước cửa sổ (1224x800), số lượng khung hình, màu sắc và lượng máu tối đa của nhân vật (5 máu).
*   **`Entity` (`Entity.h`, `Entity.cpp`)**: Lớp cơ sở (Base class) quản lý các thuộc tính vật lý chung của mọi vật thể trong game như tọa độ (x, y), kích thước, kết cấu hình ảnh (texture), và hệ thống chuyển động khung hình (animation).
*   **`Cat` (`mainObject.h`, `mainObject.cpp`)**: Lớp nhân vật chính kế thừa từ `Entity`, được bổ sung thêm thuộc tính quản lý máu (Health).
*   **`Ghost` (`Ghost.h`, `Ghost.cpp`)**: Lớp quái vật kế thừa từ `Entity`. Tích hợp thuật toán tính khoảng cách và tự động di chuyển bám theo mục tiêu, cùng các trạng thái tấn công hoặc đã chết.
*   **`Game` (`Game.h`, `Game.cpp`)**: Lớp trung tâm quản lý toàn bộ logic hệ thống. Xử lý khởi tạo SDL, tải tài nguyên, chuyển đổi trạng thái trò chơi (GameState), nhận sự kiện bàn phím/chuột, xử lý va chạm và kết xuất đồ họa (Render) ra màn hình.

## 5. Câu lệnh dòng lệnh để chạy chương trình
*(Nếu bạn sử dụng IDE như Code::Blocks, chỉ cần mở file `.cbp` và nhấn Build & Run. Nếu bạn biên dịch thủ công qua dòng lệnh bằng MinGW, có thể sử dụng mẫu câu lệnh sau)*:

```bash
# Biên dịch các file mã nguồn thành file thực thi
g++ -I src/include -L src/lib -o main main.cpp Game.cpp Entity.cpp Ghost.cpp mainObject.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Chạy trò chơi
./main
```

## 6. Danh sách chức năng đã hoàn thành
*   **Hệ thống giao diện (UI/UX):**
    *   Màn hình Menu tương tác được bằng chuột (Hover và Click).
    *   Hiển thị thanh máu (Heart), điểm số (Score), và các dòng chữ thông báo Thắng/Thua.
*   **Hệ thống Animation đa dạng:** Nhân vật Mèo có đầy đủ các hoạt ảnh: Đứng yên (Idle), Chạy (Running), Bị thương (Hurt), Tấn công (Attack), Dùng phép (Magic), Chết (Die), và Chiến thắng (Victory).
*   **Cơ chế Chiến đấu (Combat):**
    *   Tấn công cơ bản: Click chuột trái để tiêu diệt quái vật tại vị trí con trỏ.
    *   Chiêu thức Đẩy lùi (Tornado): Nhấn giữ phím `Alt` trong 1 giây (1000ms) để gồng và đẩy lùi toàn bộ quái vật ra xa. Có biểu tượng lốc xoáy hiển thị thời gian hồi chiêu (5 giây).
    *   Chiêu thức Sấm sét (Ultimate): Nhấn giữ phím `Space` để gọi sấm sét tiêu diệt toàn bộ quái vật trên bản đồ. Có biểu tượng sấm sét theo dõi thời gian hồi chiêu.
*   **Hệ thống Quái vật (AI):** Quái vật tự động sinh ra ở hai mép màn hình một cách ngẫu nhiên và liên tục rượt đuổi người chơi. Nếu chạm vào người chơi, quái vật sẽ phát động tấn công và trừ máu người chơi.
*   **Hệ thống âm thanh:** Tích hợp nhạc nền cho từng giai đoạn và hiệu ứng âm thanh sống động (tiếng gió, sấm sét, tiếng đọc thần chú, và tiếng quái vật chết).
