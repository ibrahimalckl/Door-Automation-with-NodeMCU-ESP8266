#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

//1. Firebase veritabanı adresini, Token bilgisini ve ağ adresi bilgilerinizi giriniz.
#define FIREBASE_HOST "" // http:// veya https:// olmadan yazın
#define FIREBASE_AUTH ""

WiFiManager wm;

bool res;

//2. veritabanim adında bir firebase veritabanı nesnesi oluşturuyoruz
FirebaseData veritabanim;
#define firebaseControl "{\"led\":\"1\"}"

void setup()
{

  Serial.begin(115200);

  //////////////WIFI MANAGER AYAR BLOĞU//////////////////////////////////////////////////

  //wm.resetSettings(); //bu satırı açarsak hafızadaki wifi ayarlarını temizler.
  WiFi.mode(WIFI_STA); // Özellikle modu station'a ayarlıyoruz.

  res = wm.autoConnect("AkilliKapi", "3131+%6262"); // Wifimanager bağlanma satırı. Ağ adı olarak görünmesini istediğimiz
  // ismi ve belirleyeceğimiz şifreyi tanımladık. İstersek şifre de girmeyebiliriz. Korumasız ağ olarak görünür.
  
  if (!res) {
    Serial.println("Bağlantı Sağlanamadı");
    // ESP.restart();
  }
  else {
    //buraya gelmişse WiFi'ya bağlanmış demektir.
    Serial.println("Ağ Bağlantısı Kuruldu");
  }


/////////////////////////////////////////////////////////////////////////////////////////
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP adresine bağlanıldı: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  //3. Firebase bağlantısı başlatılıyor

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Ağ bağlantısı kesilirse tekrar bağlanmasına izin veriyoruz
  Firebase.reconnectWiFi(true);
 pinMode(D2,OUTPUT);
 digitalWrite(D2,LOW);
}

void loop()
{

  if(Firebase.getString(veritabanim, "/led")) //Alınacak veri tipine göre getInt, getBool, getFloat, getDouble, getString olarak kullanılabilir.
  {
    //bağlantı başarılı ve veri geliyor ise
    Serial.print("String tipinde veri alımı başarılı, veri = ");
    Serial.println(veritabanim.stringData());
    //firebaseControl = veritabanim.stringData();
    
    //cout << "1.index:" << sifre[1] << endl;
    
    if (veritabanim.stringData()==firebaseControl){
      digitalWrite(D2,HIGH);
      Serial.print("Led Yak");
      
    }
    else {
      
      digitalWrite(D2,LOW);
      Serial.print("Led Sondur");
      Serial.print(firebaseControl);
      }
    
    

  }else{
    //hata varsa hata mesajı ve nedeni yazdırılıyor

    Serial.print("Str verisi çekilemedi, ");
    Serial.println(veritabanim.errorReason());
  }
 


  
////////////////////////////////////////////////////////////////////////////////////
// firebase veritabanına veri göndermek için Firebase.setInt komutu kullanılabilir.
//if(Firebase.setInt(veritabanim, "/led", a))
//  {
//    //bağlantı başarılı ve veri geliyor ise
//     Serial.println("Int tipinde veri gönderimi başarılı");
// 
//  }else{
//    //hata varsa hata mesajı ve nedeni yazdırılıyor
//
//    Serial.print("Int tipindeki veri gönderilemedi, ");
//    Serial.println(veritabanim.errorReason());
//  }


  
}
