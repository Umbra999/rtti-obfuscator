# Simple C++ RTTI Obfuscator

This tool obfuscates all **Run-Time Type Information (RTTI)** in a given C++ binary by replacing known patterns. It can help protect against reverse engineering attempts or prevent tracking through anti-cheat systems.

### Features
- **RTTI Obfuscation:** Masks RTTI data to hinder reverse engineering.
- **Anti-Cheat Evasion:** Helps in bypassing anti-cheat systems that rely on RTTI for detection.
- **Easy to Use:** Drop the binary onto the executable to obfuscate it automatically.

### Usage
1. **Prepare the files:** Make sure you have the binary file (e.g., `.dll`) that you want to obfuscate.
2. **Run the tool:** Simply drag and drop the binary onto the executable (`rtti-obfuscator.exe`), and it will automatically modify the binary in-place.

### Example
```bash
# Drag and drop the binary file (.dll) onto the obfuscator executable
rtti-obfuscator.exe your_binary.dll
