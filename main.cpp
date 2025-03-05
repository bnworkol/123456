#include <iostream>
#include <string>
#include <curl/curl.h>
#include <SFML/Graphics.hpp>

// ฟังก์ชั่นที่ใช้ดาวน์โหลดภาพจาก URL
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ฟังก์ชั่นที่ใช้ในการดาวน์โหลดภาพ QR Code
bool downloadImage(const std::string& url, const std::string& filename) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    // เริ่มต้น libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        FILE *fp = fopen(filename.c_str(), "wb");
        if (!fp) {
            std::cerr << "ไม่สามารถเปิดไฟล์เพื่อเขียน" << std::endl;
            return false;
        }

        // ตั้งค่า curl
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            fclose(fp);
            return false;
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return true;
}

int main() {
    std::string phoneNumber;

    // รับเบอร์โทรจากผู้ใช้
    std::cout << "กรุณากรอกเบอร์โทรศัพท์พร้อมเพย์: ";
    std::cin >> phoneNumber;

    // สร้าง URL ของ QR Code
    std::string qrCodeUrl = "https://promptpay.io/" + phoneNumber + ".png";

    // ดาวน์โหลด QR Code มาเก็บในไฟล์
    std::string filename = "qr_code.png";
    if (downloadImage(qrCodeUrl, filename)) {
        std::cout << "ดาวน์โหลด QR Code สำเร็จ!" << std::endl;

        // ใช้ SFML เพื่อแสดงภาพ
        sf::RenderWindow window(sf::VideoMode(400, 400), "QR Code");

        // โหลดภาพจากไฟล์
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            std::cerr << "ไม่สามารถโหลดภาพ QR Code" << std::endl;
            return -1;
        }

        sf::Sprite sprite(texture);

        // แสดงภาพในหน้าต่าง
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.display();
        }
    } else {
        std::cerr << "ไม่สามารถดาวน์โหลด QR Code" << std::endl;
    }

    return 0;
}
