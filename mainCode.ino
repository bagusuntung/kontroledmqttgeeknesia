//Import library dan deklarasi program.

#include <SPI.h>
#include <Ethernet.h>
#include "./PubSubClient.h"

// Settingan network(IP Address).
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte ip[]     = { 192, 168, 0, 101 };
byte dns2[]     = {8,8,8,8};
byte gateway[]     = { 192, 168, 0, 1 };
char pubschar [20];
char message_buff[250];
char message_buff_call[200];

// Tambahkan PubSubClient object
EthernetClient ethClient;
PubSubClient client("geeknesia.com", 1883, callback, ethClient);

//Dibagian ini adalah bagian setup pada program, setup baud rate 
// serial,setupEthernet,atau menambahkan serial print debugging 
// program.
int led = 8;
void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip,dns2,gateway);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
}

// Jika mqtt client belum terkoneksi maka harus menjalankan fungsi
// connect dengan parameter device id yang ada pada detail device
// dan melakukan subscribe pada topic khusus untuk menerima pesan
// perintah untuk device tersebut
// perintah client.loop digunakan untuk memeriksa koneksi mqtt secara
// berkala dan mengubah return value dari fungsi client.connected

void loop() {
  if (!client.connected())
  {
    client.connect("device-e945596e416f0f5c596a594844536b03");
    client.subscribe("topic-e945596e416f0f5c596a594844536b03");
   }
  client.loop();
}

// Fungsi ini yang akan dipanggil oleh MQTT jika menerima data
// Pesan yang dikirim melalui MQTT tersimpan pada payload
// Oleh karena itupayload lah yang akan menampung perintah
// untuk device tersebut

void callback(char* topic, byte* payload, unsigned int length) 
{ 
  int i = 0; for(i=0; i<length; i++) 
  {
    message_buff_call[i] = payload[i];
  }
  message_buff_call[i] = '\0';
  String msgString = String(message_buff_call);
  Serial.println(msgString);

  if(msgString.equals("ON")) 
  { 
    Serial.println("ON");
    digitalWrite(led,HIGH);
  } 
  if(msgString.equals("OFF")) 
  {
    Serial.println("OFF");
    digitalWrite(led,LOW);
  }
}
