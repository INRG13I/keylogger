# Cross-Platform Keylogger (macOS)

This project is a simple **keylogging utility for macOS**, built in C using CoreGraphics and Carbon APIs.  
It was developed purely for **educational and research purposes**, as part of my learning process in low-level system programming and security tooling.  

---

## 🚀 Features
- Logs **all keystrokes** on macOS using `CGEventTapCreate`.  
- **Debounce logic** to prevent duplicate key events.  
- **Dynamic log file names** (include hostname and timestamp).  
- Captures **special keys** (Shift, Cmd, Fn, arrows, etc.).  
- Runs in background with console hidden.  
- Generates **time-stamped logs** useful for forensic or debugging scenarios.  

---

## 🛠️ Tech Stack
- **Language**: C  
- **APIs**: CoreGraphics, Carbon (macOS)  
- **System**: Tested on macOS  

---

## 🎯 Objectives
This project was created to:  
- practice low-level programming on macOS;  
- understand how key events can be intercepted;  
- explore forensic tooling and red-team style techniques;  
- document findings in a structured way.  

---

## ⚠️ Disclaimer
This project is provided **for educational and demonstration purposes only**.  
It must **not** be used on any system without explicit authorization.  
Unauthorized use of keyloggers is **illegal**.  

---

## 🙏 Acknowledgments
This project was **inspired by other open-source macOS keylogger implementations**, such as [Laurin Paech’s Keylogger](https://github.com/laurinpaech/Keylogger).  
The implementation here is written from scratch and includes additional features like debouncing, hostname-based log files, and improved error handling.  

---

## 📜 License
Released under the **MIT License** – see [LICENSE](./LICENSE) for details.  

---

## 👤 Author
Created by **Raoul-Gabriel Nicolaescu** as part of my cybersecurity learning journey.  
You can connect with me on [LinkedIn](https://linkedin.com/in/raoul-nicolaescu) or explore more projects on [GitHub](https://github.com/INRG13I).
