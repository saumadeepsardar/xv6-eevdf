Based on the detailed report provided, here is a structured `README.md` file tailored for a GitHub repository.

-----

# EEVDF Scheduler for XV6

[cite\_start]This repository contains an enhanced version of the MIT XV6 operating system, replacing the default Round-Robin scheduler with an **Earliest Eligible Virtual Deadline First (EEVDF)** scheduler[cite: 1, 18].

[cite\_start]This project was developed as an Operating Systems Course Project at the Indian Institute of Information Technology Design and Manufacturing (IIITDM), Kancheepuram[cite: 3, 8, 9].

## 📖 Introduction

[cite\_start]EEVDF is an optimal proportional-share CPU scheduling algorithm, similar to mechanisms used in modern Linux kernels (like CFS)[cite: 25]. Unlike the default XV6 scheduler, which can lead to unfair CPU distribution, this implementation ensures:

  * [cite\_start]**Proportional Fairness:** Processes receive CPU time proportional to their assigned weights[cite: 27].
  * [cite\_start]**Starvation Freedom:** The algorithm uses virtual deadlines to ensure every process eventually runs[cite: 35].
  * [cite\_start]**Low Latency:** Optimized for interactivity and predictable deadline-based selection[cite: 34, 36].

The scheduler tracks a "Virtual Runtime" ($v_{runtime}$) and a "Virtual Deadline" ($v_{deadline}$) for every process. [cite\_start]The process with the earliest eligible virtual deadline is selected for execution[cite: 63, 68].

## ⚙️ Key Modifications

The implementation spans both kernel-space logic and user-space APIs. Below is a list of files modified or created for this project:

### Kernel Space

| File | Modification Description |
| :--- | :--- |
| **`kernel/proc.h`** | [cite\_start]Added fields to `struct proc` including `vruntime`, `vdeadline`, `weight`, and statistics counters (`sched_count`, `sleep_time`)[cite: 71, 74]. |
| **`kernel/proc.c`** | Replaced the `scheduler()` loop with EEVDF logic. [cite\_start]Added helper functions `setweight()` and `getpinfo()`[cite: 94, 198, 206]. |
| **`kernel/sysproc.c`** | [cite\_start]Implemented system calls: `sys_setweight`, `sys_setschedtrace`, and `sys_getpinfo`[cite: 223, 233, 242]. |

### User Space

| File | Modification Description |
| :--- | :--- |
| **`user/user.h`** | [cite\_start]Added function prototypes for user-level API calls: `setweight`, `getpinfo`, and `setschedtrace`[cite: 259, 260]. |
| **`user/eevdf_test1.c`** | [cite\_start]**Test 1:** CPU Load Test (Heavy compute without yielding)[cite: 266]. |
| **`user/eevdf_test2.c`** | [cite\_start]**Test 2:** Mixed CPU + Sleep Test (Validating behavior with voluntary yields)[cite: 358]. |
| **`user/eevdf_test3.c`** | [cite\_start]**Test 3:** Sleep/Wakeup Fairness Test (Simulating I/O bound behavior)[cite: 503]. |

## 🛠️ System Calls

The following system calls were introduced to interact with the scheduler:

1.  **`setweight(int weight)`**: Sets the proportional weight of the current process. [cite\_start]Higher weight = more CPU time[cite: 226].
2.  [cite\_start]**`setschedtrace(int enable)`**: Enables real-time logging of scheduling events to the console for debugging[cite: 235].
3.  [cite\_start]**`getpinfo(struct pinfo *info)`**: Retrieves internal scheduler state (runtime, deadline, sched\_count) for the current process[cite: 245].

## 🚀 How to Run (Linux)

Follow these steps to compile and run the modified XV6 kernel on a Linux environment.

### 1\. Prerequisites

Ensure you have the RISC-V toolchain and QEMU installed. On Debian/Ubuntu:

```bash
sudo apt-get update
sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
```

### 2\. Build and Launch

Navigate to the source directory and compile the kernel using `make`:

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

[cite\_start]To enable verbose scheduling traces manually inside any program, the code uses `setschedtrace(1)`[cite: 281].

## 👥 Contributors

  * [cite\_start]**Saumadeep Sardar** (Roll No.: CS23B1049) [cite: 5]
  * [cite\_start]**Dhage Pratik Bhishmacharya** (Roll No.: CS23B1047) [cite: 6]
  * [cite\_start]**Nisarg Ranade** (Roll No.: CS23B1090) [cite: 6]