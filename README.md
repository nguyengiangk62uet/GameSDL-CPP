# GameSDL-CPP
<h2>Tạo Game bắn gà đơn giản với SDL</h3>

- Game bắn gà dựa trên cảm hứng từ thể loại game bắn gà huyền thoại
- Mã nguồn được viết bằng ngôn ngữ C++ đồ họa SDL 1.2.15.

Nhiệm vụ: Bắn các đối tượng quái để lấy điểm, hạ gục boss để chiến thắng.

<b>Các đối tượng trong game:</b>
- Máy bay bắn gà với 2 loại đạn
- Đối tượng quái: Gà nhỏ, boss
- Mạng của người chơi, điểm số
- Sử dụng SDL_Image, SDL_TTF, SDL_Mixer

Cần nắm được các class trong game:
- Một lớp chứa các thuộc tính chung, các hàm khởi tạo ban đầu (Common_Function.h)
- Đối tượng tổng, các đối tượng sau đều kế thừa từ đối tượng này (BaseObject)
- Nhân vật chính - máy bay (MainObject)
- Các đối tượng quái vật (ThreatsObject)
- Lớp đạn của máy bay và quái vật (AmoObject)
- Lớp thể hiện sự kiện va chạm (ExplosionObject)
- Lớp thể hiện máu, mạng của người chơi (HealthObject)
- Lớp có chức năng load text lên screen (TextObject)
---------
- Các sự kiện va chạm giữa người với quái, bắn trúng quái, người dính đạn của quái.
- Phương thức di chuyển của người và quái
