
# ESP32-CAM Tank Robot Dokumentáció

## 1. Felhasználói platform bemutatása
Az **ESP32-CAM** egy fejlett, ESP32-alapú fejlesztői modul, amely beépített Wi-Fi és kamera képességekkel rendelkezik. Ideális választás IoT-, AI- és robotikai alkalmazásokhoz. Ez a projekt kihasználja az ESP32-CAM képességeit a motorvezérléshez, a kamera képének streameléséhez, és egy egyszerű webes interfész biztosításához.

### **ESP32-CAM főbb jellemzői**
- **Processzor:** Dual-core Xtensa LX6, akár 240 MHz órajellel.
- **Memória:** 520 KB belső SRAM + opcionális PSRAM (8 MB).
- **Csatlakoztathatóság:** Beépített Wi-Fi (802.11 b/g/n) és Bluetooth.
- **Kamera:** OV2640 támogatás, VGA (640x480) felbontás.
- **GPIO-k:** Rugalmas csatlakozási lehetőségek (pl. motorvezérlés, kameraadatok).
- **Tápfeszültség:** 5V, azonban a projektben 12V-ot használunk a motorok számára.

### **Felhasználási területek**
Az ESP32-CAM gyakran alkalmazott:
- Robotikában (pl. robotkarok, tankok, járművek).
- Biztonsági rendszerekben (IP-kamerák).
- Automatizált IoT eszközökben.

---

## 2. Tervdokumentáció
### **Kapcsolási rajz és felépítés**
A projektben két pár DC motort használunk, amelyeket egy **L298N H-híd** vezérlő hajt meg. Az ESP32-CAM vezérli a motorokat, valamint a kamera képét streameli. Az alábbi kapcsolási pontok biztosítják az eszköz megfelelő működését:

#### **Kapcsolódások**
1. **ESP32-CAM és motorvezérlő (L298N):**
   - **Bal oldali motor (Motor 1):**
     - IN1 -> GPIO 14
     - IN2 -> GPIO 15
     - ENA -> GPIO 1 (PWM)
   - **Jobb oldali motor (Motor 2):**
     - IN1 -> GPIO 13
     - IN2 -> GPIO 12
     - ENB -> GPIO 2 (PWM)
2. **Kamera kapcsolódása az ESP32-CAM-hez:**
   - Gyári lábkiosztás szerint kötve (lásd `camera_pins.h`).

#### **Energiaellátás**
A motorvezérlő és az ESP32-CAM 12V-os tápegységről működik. Az L298N biztosítja a motorok megfelelő működéséhez szükséges áramot.

### **Alkatrészlista**
| Alkatrész         | Mennyiség | Leírás                           |
|-------------------|-----------|----------------------------------|
| ESP32-CAM         | 1         | Kamera és vezérlőegység         |
| L298N H-híd       | 1         | DC motorvezérlés                |
| DC motor (6-12V)  | 4         | Tank meghajtás                  |
| 12V tápegység     | 1         | Energiaellátás                  |
| Jumper kábelek    | Több      | Csatlakozások                   |
| Alaplap vagy tartó | 1         | Stabil platform                 |

---

## 3. Megvalósítás és működés leírása
A robot tankstílusú meghajtással működik, amelyet külön motorok vezérelnek. Az ESP32-CAM lehetővé teszi a kamera képének streamelését és a motorok irányítását a felhasználói parancsok alapján.

### **Működési módok**
- **Előre haladás:** Mindkét oldal motorjai azonos sebességgel forognak előre.
- **Hátrafelé haladás:** Mindkét oldal motorjai azonos sebességgel forognak hátrafelé.
- **Jobbra/Balra fordulás:** Az egyik oldalon előre, a másik oldalon hátrafelé forognak a motorok.
- **Enyhe kanyarodás:** Az egyik oldal motorjai csökkentett sebességgel forognak.
- **Stop:** Az összes motor leáll.

#### **Kamera funkciók**
- Élőkép streamelése MJPEG formátumban.
- A webes interfész automatikusan beállítja a kamera URL-jét.

### **Felhasználói élmény**
A webes interfész lehetővé teszi:
1. A robot irányítását egyszerű gombokkal.
2. Az élő kamerakép megtekintését.
3. Az egyes gombok állapotának valós idejű frissítését.

---

## 4. Beágyazott vezérlő programok
A vezérlés főbb komponensei:
1. **Motorvezérlő osztály (`DCMotor`)**:
   - Motorok sebességét és irányát PWM-mel szabályozza.
   - Támogatja az időzített motorvezérlést (pl. `on(speed, millisec)`).

2. **Callback funkció (`action_callback`)**:
   - HTTP parancsokat dolgoz fel (pl. előre, hátra, jobbra, balra).

3. **Kamera stream handler**:
   - Az `/stream` végponton keresztül továbbítja az MJPEG élőképet.

**Példa: Motorvezérlés megvalósítása:**
```cpp
if (!strcmp(variable, "forward")) {
  motorRight.on(MAX_SPEED);
  motorLeft.on(MAX_SPEED);
}
```

---

## 5. Felhasználói interfész programok
A webes interfész egy HTML5 oldalt tartalmaz, amely JavaScript segítségével HTTP-kéréseket küld az ESP32-CAM-nek. A felhasználói élmény javítása érdekében a gombok állapota dinamikusan frissül.

### **Felépítés**
- **Kamera stream:** A weboldal automatikusan betölti az ESP32-CAM kamera képét.
- **Irányítási gombok:** Előre, hátra, jobbra, balra, stop.
- **Aszinkron parancsok:** A JavaScript XMLHttpRequest használatával kommunikál a vezérlővel.

**Példa gombok működésére:**
```html
<button id="forward" class="button" 
  onmousedown="pressButton('forward');" 
  onmouseup="releaseButton('forward');">Forward</button>
```

---

## 6. Programok forráskódja és dokumentációja
A projekt kódja több fájlból áll:
- `camera.ino`: A vezérlés fő programja.
- `stream.cpp`: A kamera adatfolyamának kezelése.
- `html.h`: A webes interfész HTML kódja.

**Példa: Stream inicializálása**
```cpp
void initCameraServer() {
  // Kamera konfigurációja
  esp_camera_init(&config);
  
  // Wi-Fi csatlakozás
  WiFi.begin(ssid, password);
  
  // Stream szerver indítása
  startCameraServer(action_callback);
}
```

---

### Következtetések és tapasztalatok
1. Az ESP32-CAM kiválóan alkalmas robotikai projektekhez, kombinálva a kamera és a vezérlési funkciókat.
2. Az L298N H-híd egyszerű és hatékony módja a motorvezérlésnek.
3. A webes interfész gyorsan testreszabható további funkciókkal (pl. sebességállítás, LED vezérlés).


