# [cite_start]Better Call Saul API using Bluetooth Classic [cite: 1]

[cite_start]Acest proiect a fost realizat de către **Vincene Eduard** [cite: 2] [cite_start]și **Enciu Andrei Eduard** [cite: 3] [cite_start]din grupa **ETTI-CTI-413A**[cite: 4].

## 📝 Descriere Generală
[cite_start]Proiectul presupune conectarea unui telefon mobil prin intermediul **Bluetooth Classic** la o aplicație denumită „Proiect IA”[cite: 5]. [cite_start]Prin această conexiune, se realizează configurarea plăcuței **ESP32** pentru acces la internet (WIFI), urmată de obținerea unor date de la un API web extern despre personajele din serialul *"Better Call Saul"*[cite: 5].

[cite_start]Comunicarea dintre aplicația mobilă și plăcuța ESP32 se realizează exclusiv prin schimb de date în format **JSON**, asigurând o structură flexibilă și ușor de extins[cite: 6].

## 🚀 Funcționalități Principale
* [cite_start]**Scanare rețele WIFI:** Plăcuța scanează rețelele disponibile și trimite către aplicație detalii precum SSID, puterea semnalului și tipul de criptare[cite: 12].
* [cite_start]**Conectare de la distanță:** Permite introducerea credențialelor WIFI în aplicație și transmiterea lor către ESP32 pentru autentificare[cite: 15].
* [cite_start]**Interogare API:** Plăcuța face cereri HTTP GET pentru a prelua lista de personaje și detaliile acestora[cite: 18, 24].
* [cite_start]**Transmitere date prin Bluetooth:** Toate informațiile procesate (liste de rețele, confirmări de conexiune, date API) sunt transmise înapoi la telefon prin Bluetooth[cite: 13, 16, 19, 25].

## 🛠️ Structura Codului (Funcții Cheie)

### 1. Procesarea Comenzilor (`receivedData`)
[cite_start]Funcția principală de control care primește date prin Bluetooth și, în funcție de acțiunea specificată în JSON (`getNetworks`, `connect`, `getData`, `getDetails`), apelează logica corespunzătoare[cite: 8, 9, 10].

### 2. Gestionarea WIFI (`Lista_Retele` & `conecteaza`)
* [cite_start]**`Lista_Retele`**: Scanează mediul local și construiește un obiect JSON pentru fiecare rețea găsită[cite: 11, 12].
* **`conecteaza`**: Primește SSID-ul și parola, încearcă stabilirea conexiunii și raportează succesul sau eșecul prin Bluetooth[cite: 14, 15, 16].

### 3. Integrarea cu API-ul (`get_data` & `detalii_extra`)
* [cite_start]**`get_data`**: Realizează o cerere GET către server pentru a obține lista generală de personaje (nume, ID, imagine)[cite: 17, 18, 21].
* **`detalii_extra`**: Folosește un ID specific pentru a aduce informații amănunțite precum: data nașterii, ocupația, statusul, pseudonimul și sezoanele în care apare personajul[cite: 22, 23, 24].

## 📊 Date Preluate (Exemple)
[cite_start]Aplicația afișează o listă interactivă cu personaje cunoscute[cite: 20]:
* [cite_start]**Howard Hamlin** (ID: 113) [cite: 21]
* **Gustavo Fring** (ID: 9) [cite: 21]
* [cite_start]**Kimberly Wexler** (ID: 112) [cite: 21]
* [cite_start]**Mike Ehrmantraut** (ID: 7) [cite: 21]

---
[cite_start]**Tehnologii utilizate:** ESP32, Bluetooth Classic, JSON (ArduinoJson), HTTPClient[cite: 1, 6, 8, 17].
