# WSL Installation Guide for Your Linux Shell Project

## 🚀 Quick Installation Guide

### Method 1: Using Windows Features (Recommended)

#### Step 1: Enable WSL Feature
1. **Open Windows Settings**
   - Press `Win + I` or search for "Settings"
   
2. **Navigate to Apps**
   - Click on **Apps** → **Optional Features** → **More Windows Features**
   - OR search for "Turn Windows features on or off" in Start menu

3. **Enable Required Features**
   - ✅ Check **"Windows Subsystem for Linux"**
   - ✅ Check **"Virtual Machine Platform"** (for WSL 2)
   - Click **OK**
   
4. **Restart Your Computer** (Required!)

---

#### Step 2: Install Ubuntu from Microsoft Store

1. **Open Microsoft Store**
   - Press `Win + S` and search for "Microsoft Store"
   
2. **Search for Ubuntu**
   - In the Store, search for **"Ubuntu"**
   - Choose **"Ubuntu 22.04 LTS"** or **"Ubuntu"** (latest)
   
3. **Install Ubuntu**
   - Click **"Get"** or **"Install"**
   - Wait for download to complete (may take 5-10 minutes)

4. **Launch Ubuntu**
   - Click **"Open"** in Microsoft Store
   - OR search for "Ubuntu" in Start menu

---

#### Step 3: First-Time Setup

1. **Wait for Installation**
   - First launch will take 2-5 minutes
   - You'll see: "Installing, this may take a few minutes..."

2. **Create Linux User**
   ```
   Enter new UNIX username: [your-username]
   New password: [create-a-password]
   Retype new password: [same-password]
   ```
   - **Important**: Remember this password! You'll need it for `sudo` commands

3. **Success!**
   - You should see a terminal prompt like: `username@computername:~$`

---

### Method 2: Using PowerShell (Alternative - Requires Admin)

If Method 1 doesn't work, try this:

1. **Open PowerShell as Administrator**
   - Right-click Start menu → **"Windows PowerShell (Admin)"**

2. **Run Installation Command**
   ```powershell
   wsl --install -d Ubuntu
   ```

3. **Restart Computer** when prompted

4. **Launch Ubuntu** from Start menu after restart

---

## 🔧 After Installation: Setup for Your Shell Project

### Step 1: Update Ubuntu
```bash
sudo apt update
sudo apt upgrade -y
```

### Step 2: Install Build Tools (GCC, Make, etc.)
```bash
sudo apt install build-essential -y
```

This installs:
- ✅ GCC compiler
- ✅ Make
- ✅ Standard C libraries
- ✅ Development tools

### Step 3: Navigate to Your Project
```bash
# Windows drives are mounted at /mnt/
cd /mnt/d/Projects/simple-shell
```

### Step 4: Compile Your Shell
```bash
make
```

### Step 5: Run Your Shell
```bash
./shell
```

---

## 📁 Understanding File Access

### Windows → Linux
Your Windows drives are accessible in WSL at:
- **C:\** → `/mnt/c/`
- **D:\** → `/mnt/d/`
- **Your project:** `/mnt/d/Projects/simple-shell`

### Linux → Windows
Your WSL files are at:
- **Windows Explorer:** `\\wsl$\Ubuntu\home\your-username\`

---

## 🎯 Quick Commands Reference

### Open WSL Terminal
```
Method 1: Search "Ubuntu" in Start menu
Method 2: Open PowerShell and type: wsl
Method 3: Windows Terminal → Ubuntu tab
```

### Navigate to Your Project
```bash
cd /mnt/d/Projects/simple-shell
```

### Compile and Run
```bash
make          # Compile
./shell       # Run your shell
make clean    # Clean build files
make test     # Run tests
```

### Exit WSL
```bash
exit
```

---

## 🐛 Troubleshooting

### Issue 1: "WSL 2 requires an update to its kernel component"
**Solution:**
1. Download: https://aka.ms/wsl2kernel
2. Install the update
3. Restart computer

### Issue 2: "The Windows Subsystem for Linux optional component is not enabled"
**Solution:**
1. Go to "Turn Windows features on or off"
2. Enable "Windows Subsystem for Linux"
3. Restart computer

### Issue 3: "gcc: command not found"
**Solution:**
```bash
sudo apt update
sudo apt install build-essential -y
```

### Issue 4: "Permission denied" when running ./shell
**Solution:**
```bash
chmod +x shell
./shell
```

---

## ✅ Verification Checklist

After installation, verify everything works:

```bash
# Check WSL version
wsl --version

# Check Ubuntu is running
wsl -l -v

# Inside WSL, check tools:
gcc --version        # Should show GCC version
make --version       # Should show Make version
pwd                  # Should show current directory
```

---

## 🎓 Your Next Steps

1. ✅ Install WSL using Method 1 above
2. ✅ Restart your computer
3. ✅ Launch Ubuntu and create user
4. ✅ Install build tools: `sudo apt install build-essential -y`
5. ✅ Navigate to project: `cd /mnt/d/Projects/simple-shell`
6. ✅ Compile: `make`
7. ✅ Run: `./shell`
8. ✅ Test all 40 commands!

---

## 📞 Need Help?

If you get stuck at any step, let me know:
- Which step you're on
- What error message you see
- Screenshot if possible

---

**Good luck! Your shell project is ready to run once WSL is installed! 🚀**
