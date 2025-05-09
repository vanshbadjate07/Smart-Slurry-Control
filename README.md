# 🌾 Automated Slurry Utilization and Waste Recycling in Smart Agriculture

## 🚀 Project Overview
This project focuses on building an **IoT-driven smart irrigation system** that automates **slurry distribution** in agricultural fields. By using real-time soil moisture data and Firebase integration, the system intelligently manages slurry flow line by line, enhancing efficiency, reducing manual labor, and promoting sustainable farming.

---

## 🔧 What the Project Does
- Automatically detects soil dryness and distributes slurry accordingly.
- Activates and deactivates slurry valves in sequence (Line 1 to Line 4).
- Sends current line status to Firebase in real-time.
- Website fetches data from Firebase to display live irrigation status.

---

## ⚙️ How It Works
1. ESP32 collects data from **soil moisture sensors** and controls **relays** connected to slurry valves and motors.
2. Based on sensor readings, slurry is pumped line by line until the desired moisture level is reached.
3. **Firebase** acts as a bridge to store and sync real-time data.
4. A **web dashboard** fetches the current status from Firebase and shows which irrigation line is active.

---

## 🔩 Hardware Components Used
- ESP32 Microcontroller
- 4 Soil Moisture Sensors
- 4-Channel Relay Module
- 3 Motors (Slurry Distribution + Mixing)
- 4 Solenoid Valves (Line Control)
- Jumper Wires & Breadboard
- Power Supply
- Firebase (for real-time cloud integration)

---

## 🌐 Website Features
### 📊 What the Website Shows:
- Real-time active slurry line (1 to 4)
- System connection status
- Last update timestamp
- System health overview

### Live Preview
- [Smart-Slurry-Control](https://vanshbadjate07.github.io/Smart-Slurry-Control/)

### � What's Included:
- Stylish, responsive UI
- Real-time Firebase data fetching
- Animated pipe status for active lines
- Dashboard with line-by-line visualization

---

## 🔗 Firebase Integration
### ESP32 to Firebase:
- ESP32 pushes the current active line status to Firebase Realtime Database using the `FirebaseESP32` library.

### Firebase to Website:
- Website uses **Firebase JavaScript SDK** to fetch the `/currentLine` value in real time and update the UI accordingly.

---

## 👨‍💻 Group Members
- **Vansh Badjate** [LinkedIn](https://www.linkedin.com/in/vansh-badjate1008/)
- **Abhishek Dighe**
- **Tanuja Gawali**
- **Swadesh Turkane**

---

## ✅ Conclusion
This system simplifies slurry management with **real-time automation and remote monitoring**, making farming more **efficient, eco-friendly**, and **cost-effective**. Ideal for medium-scale farms, it ensures optimal soil conditions with minimal human intervention.
