
# EEVDF Scheduler for XV6

This repository contains an enhanced version of the MIT XV6 operating system, replacing the default Round-Robin scheduler with an **Earliest Eligible Virtual Deadline First (EEVDF)** scheduler.

This project was developed as an Operating Systems (CS3000) Course Project at the Indian Institute of Information Technology Design and Manufacturing (IIITDM) Kancheepuram, Chennai.

## 📖 Introduction

EEVDF is an optimal proportional-share CPU scheduling algorithm, similar to mechanisms used in modern Linux kernels (like CFS). Unlike the default XV6 scheduler, which can lead to unfair CPU distribution, this implementation ensures:

  * **Proportional Fairness:** Processes receive CPU time proportional to their assigned weights.
  * **Starvation Freedom:** The algorithm uses virtual deadlines to ensure every process eventually runs.
  * **Low Latency:** Optimized for interactivity and predictable deadline-based selection.

The scheduler tracks a "Virtual Runtime" ($v_{runtime}$) and a "Virtual Deadline" ($v_{deadline}$) for every process. The process with the earliest eligible virtual deadline is selected for execution.

## ⚙️ Key Modifications

The implementation spans both kernel-space logic and user-space APIs. Below is a list of files modified or created for this project:

### Kernel Space

| File | Modification Description |
| :--- | :--- |
| **`kernel/proc.h`** | Added fields to `struct proc` including `vruntime`, `vdeadline`, `weight`, and statistics counters (`sched_count`, `sleep_time`). |
| **`kernel/proc.c`** | Replaced the `scheduler()` loop with EEVDF logic. Added helper functions `setweight()` and `getpinfo()`. |
| **`kernel/sysproc.c`** | Implemented system calls: `sys_setweight`, `sys_setschedtrace`, and `sys_getpinfo`. |

### User Space

| File | Modification Description |
| :--- | :--- |
| **`user/user.h`** | Added function prototypes for user-level API calls: `setweight`, `getpinfo`, and `setschedtrace`. |
| **`user/eevdf_test1.c`** | **Test 1:** CPU Load Test (Heavy compute without yielding). |
| **`user/eevdf_test2.c`** | **Test 2:** Mixed CPU + Sleep Test (Validating behavior with voluntary yields). |
| **`user/eevdf_test3.c`** | **Test 3:** Sleep/Wakeup Fairness Test (Simulating I/O bound behavior). |

## 🛠️ System Calls

The following system calls were introduced to interact with the scheduler:

1.  **`setweight(int weight)`**: Sets the proportional weight of the current process. Higher weight = more CPU time.
2.  **`setschedtrace(int enable)`**: Enables real-time logging of scheduling events to the console for debugging.
3.  **`getpinfo(struct pinfo *info)`**: Retrieves internal scheduler state (runtime, deadline, sched\_count) for the current process.

## 🚀 How to Run (Linux)

Follow these steps to compile and run the modified XV6 kernel on a Linux environment.

### 1\. Prerequisites

Ensure you have the RISC-V toolchain and QEMU installed. On Debian/Ubuntu:

```bash
sudo apt-get update
sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```

### 2\. Build and Launch

```bash
# Git clone reporsitory
git clone https://github.com/saumadeepsardar/xv6-eevdf.git

# Naviagate to project folder
cd xv6-eevdf
```
Compile the kernel using `make`:

```bash
# Clean previous builds
make clean

# Build and run QEMU
make qemu
```

### 3\. Running Tests

Once inside the XV6 shell (`$`), you can run the implemented test programs to verify the scheduler's behavior:

**Test 1: CPU Load Verification**
Spawns children with weights 1, 3, and 5 to see if they finish proportionally.

```bash
$ eevdf_test1
```

**Test 2: Mixed Workloads**
Tests interaction between CPU-bound and sleeping processes.

```bash
$ eevdf_test2
```

**Test 3: Fairness Check**
Verifies that processes waking up from sleep are treated fairly regarding deadlines.

```bash
$ eevdf_test3
```

To enable verbose scheduling traces manually inside any program, the code uses `setschedtrace(1)`.

## 👥 Contributors

  * **Saumadeep Sardar** (Roll No.: CS23B1049) 
  * **Dhage Pratik Bhishmacharya** (Roll No.: CS23B1047) 
  * **Nisarg Ranade** (Roll No.: CS23B1090) 

