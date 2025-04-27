# -- Imports --
from abc import ABCMeta, abstractmethod
from contextlib import contextmanager
import numpy as np
from typing import List
import argparse
import matplotlib.pyplot as plt

# -- Quantum primitives --

KET_0 = np.array([[1], [0]], dtype=complex)
H = np.array([[1, 1], [1, -1]], dtype=complex) / np.sqrt(2)
X = np.array([[0, 1], [1, 0]], dtype=complex)

class Qubit(metaclass=ABCMeta):
    @abstractmethod
    def h(self): pass

    @abstractmethod
    def x(self): pass

    @abstractmethod
    def measure(self) -> bool: pass

    @abstractmethod
    def reset(self): pass

class QuantumDevice(metaclass=ABCMeta):
    @abstractmethod
    def allocate_qubit(self) -> Qubit:
        pass

    @abstractmethod
    def deallocate_qubit(self, qubit: Qubit):
        pass

    @contextmanager
    def using_qubit(self):
        qubit = self.allocate_qubit()
        try:
            yield qubit
        finally:
            qubit.reset()
            self.deallocate_qubit(qubit)

class SimulatedQubit(Qubit):
    def __init__(self):
        self.reset()

    def h(self):
        self.state = H @ self.state

    def x(self):
        self.state = X @ self.state

    def measure(self) -> bool:
        pr0 = np.abs(self.state[0, 0]) ** 2
        sample = np.random.random() <= pr0
        return bool(0 if sample else 1)

    def reset(self):
        self.state = KET_0.copy()

class SingleQubitSimulator(QuantumDevice):
    def __init__(self):
        self.available_qubits = [SimulatedQubit()]

    def allocate_qubit(self) -> SimulatedQubit:
        if self.available_qubits:
            return self.available_qubits.pop()

    def deallocate_qubit(self, qubit: SimulatedQubit):
        self.available_qubits.append(qubit)

# -- Utility functions --

def prepare_classical_message(bit: bool, q: Qubit) -> None:
    if bit:
        q.x()

def sample_random_bit(device: QuantumDevice) -> bool:
    with device.using_qubit() as q:
        q.h()
        result = q.measure()
        q.reset()
    return result

def prepare_message_qubit(message: bool, basis: bool, q: Qubit) -> None:
    if message:
        q.x()
    if basis:
        q.h()

def measure_message_qubit(basis: bool, q: Qubit) -> bool:
    if basis:
        q.h()
    result = q.measure()
    q.reset()
    return result

def convert_to_hex(bits: List[bool]) -> str:
    return hex(int("".join(["1" if bit else "0" for bit in bits]), 2))

def apply_one_time_pad(message: List[bool], key: List[bool]) -> List[bool]:
    return [m ^ k for m, k in zip(message, key)]

def string_to_bits(message: str) -> List[bool]:
    return [bit == '1' for byte in message.encode('utf-8') for bit in format(byte, '08b')]

def bits_to_string(bits: List[bool]) -> str:
    bytes_data = []
    for i in range(0, len(bits), 8):
        byte = bits[i:i+8]
        if len(byte) == 8:
            byte_val = int(''.join('1' if b else '0' for b in byte), 2)
            bytes_data.append(byte_val)
    return bytes(bytes_data).decode('utf-8', errors='replace')

# -- BB84 transmission functions --

def send_single_bit_with_bb84(your_device: QuantumDevice, bob_device: QuantumDevice):
    your_message = sample_random_bit(your_device)
    your_basis = sample_random_bit(your_device)
    bob_basis = sample_random_bit(bob_device)

    with your_device.using_qubit() as q:
        prepare_message_qubit(your_message, your_basis, q)
        bob_measurement = measure_message_qubit(bob_basis, q)

    return (your_message, your_basis), (bob_measurement, bob_basis)

def send_single_bit_with_bb84_realistic(your_device: QuantumDevice, eve_device: QuantumDevice, bob_device: QuantumDevice):
    your_message = sample_random_bit(your_device)
    your_basis = sample_random_bit(your_device)

    eve_basis = sample_random_bit(eve_device)
    bob_basis = sample_random_bit(bob_device)

    with your_device.using_qubit() as q:
        prepare_message_qubit(your_message, your_basis, q)

        # Eve intercepts
        eve_measurement = measure_message_qubit(eve_basis, q)
        prepare_message_qubit(eve_measurement, eve_basis, q)  # Eve resends the qubit

        # Bob receives and measures
        bob_measurement = measure_message_qubit(bob_basis, q)

    return (your_message, your_basis), (bob_measurement, bob_basis)

# -- Simulation functions --

def simulate_bb84_realistic(n_bits: int, keyFor: str, attm=1, eavesdrop=False):
    your_device = SingleQubitSimulator()
    eve_device = SingleQubitSimulator()
    bob_device = SingleQubitSimulator()

    key = []
    n_rounds = 0
    errors = 0
    sample_size = max(1, n_bits // 5)  # 20% for testing

    while len(key) < n_bits + sample_size:
        n_rounds += 1
        if eavesdrop:
            ((your_message, your_basis), (bob_measurement, bob_basis)) = send_single_bit_with_bb84_realistic(your_device, eve_device, bob_device)
        else:
            ((your_message, your_basis), (bob_measurement, bob_basis)) = send_single_bit_with_bb84(your_device, bob_device)

        if your_basis == bob_basis:
            key.append((your_message, bob_measurement))

    sample = key[:sample_size]
    key = key[sample_size:]

    for (your_bit, bob_bit) in sample:
        if your_bit != bob_bit:
            errors += 1

    if attm < 20:
        colrStr = f"\33[48;5;{n_rounds%216}m\33[38;5;{(n_rounds+15)%216}m"
        print(f"Took {colrStr}{n_rounds}\33[0m rounds to generate a {n_bits}-bit key for {colrStr}{keyFor}\33[0m.")

    if errors > 0:
        print(f"\n\33[101mWARNING:\33[0m Detected {errors} errors in the public sample! Likely Eavesdropping!")
    else:
        print(f"\n\33[102mNo errors detected. Key is considered secure.\33[0m")

    final_key = [your_bit for (your_bit, bob_bit) in key]
    return final_key

# -- Eavesdropping detection statistics --
def experiment_eavesdropping_detection(num_trials: int, n_bits: int):
    detections = 0
    total_errors = []

    print(f"\nRunning {num_trials} experiments with {n_bits}-bit keys and Eve eavesdropping...\n")

    for trial in range(num_trials):
        your_device = SingleQubitSimulator()
        eve_device = SingleQubitSimulator()
        bob_device = SingleQubitSimulator()

        key = []
        n_rounds = 0
        errors = 0
        sample_size = max(1, n_bits // 5)

        while len(key) < n_bits + sample_size:
            n_rounds += 1
            ((your_message, your_basis), (bob_measurement, bob_basis)) = send_single_bit_with_bb84_realistic(your_device, eve_device, bob_device)
            if your_basis == bob_basis:
                key.append((your_message, bob_measurement))

        sample = key[:sample_size]
        key = key[sample_size:]

        for (your_bit, bob_bit) in sample:
            if your_bit != bob_bit:
                errors += 1

        total_errors.append(errors)

        if errors > 0:
            detections += 1  # Eve detected

    detection_rate = detections / num_trials * 100

    print(f"\n\33[95mEve was detected in {detections}/{num_trials} runs ({detection_rate:.2f}% detection rate).\33[0m")

    # Optional: Plot histogram
    plt.hist(total_errors, bins=range(0, max(total_errors)+2), align='left', rwidth=0.8)
    plt.title("Distribution of Errors Caused by Eve")
    plt.xlabel(f"Number of Errors Detected ({n_bits}-bit key, {num_trials} trials)")
    plt.ylabel("Frequency")
    plt.grid(axis='y')
    plt.show()

# -- Main runner --

def printUsage():
    usage = """
Usage: python bb84.py
Optional arguments:
  -n <attempts> : number of decryption attempts by Eve (default 1)
  -m <message>  : secret message to be sent
  -e           : simulate Eve eavesdropping during key exchange
  -h           : show this help message and exit
"""
    print(usage)
import random
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Realistic BB84 QKD Simulation")
    parser.add_argument("-n", "--attempts", type=int, default=1, help="Number of decryption attempts by Eve")
    parser.add_argument("-m", "--message", type=str, help="Message to be sent")
    parser.add_argument(
        "-e", "--eavesdrop", action="store_true",
        help="If set, simulate Eve eavesdropping during key exchange"
    )
    try:
        args = parser.parse_args()
    except:
        printUsage()
        exit(1)

    if args.attempts < 1:
        print("Error: Number of decryption attempts must be at least 1.")
        exit(1)

    try:
        # Step 1: Message Setup
        if args.message:
            message = args.message
            print(f"Using provided secret message: \33[1;38;5;82m{message}\33[0m")
        else:
            message = input("Enter your secret message: \33[1;38;5;82m")
            print("\33[0m")

        message_bits = string_to_bits(message)
        required_bits = len(message_bits)

        # Step 2: Key Generation
        print(f"\n\33[48;5;18;38;5;33mGenerating a {required_bits}-bit key by simulating BB84...\33[0m")
        key = simulate_bb84_realistic(required_bits, "Bob", eavesdrop=args.eavesdrop)

        print(f"Got key:                          {convert_to_hex(key)}")
        print(f"Original message bits:            {convert_to_hex(message_bits)}")

        # Step 3: Encryption
        encrypted_message = apply_one_time_pad(message_bits, key)
        print(f"Encrypted message:                \33[1m\33[38;5;33m{convert_to_hex(encrypted_message)}\33[0m\n")

        # Step 4: Eve's Decryption Attempts
        eveSucceeded = False
        for i in range(args.attempts):
            eve_key = simulate_bb84_realistic(required_bits, "Eve", args.attempts,
                                              True)  # Always eavesdrop for Eve's guessing  #(args.eavesdrop and random.random()<0.3)
            eve_attempt = apply_one_time_pad(encrypted_message, eve_key)
            print(f"\33[31mEve's\33[0m decryption attempt {i+1}:")
            if args.attempts < 20:
                print(f"Eve's random key:                 {convert_to_hex(eve_key)}")
                print(f"Eve's decrypted bits:             {convert_to_hex(eve_attempt)}")
            try:
                eve_text = bits_to_string(eve_attempt)
                print(f"Eve's decrypted text:            \33[1;38;5;124m{eve_text}\33[0m")
            except:
                print("Eve's decrypted text:            [Invalid UTF-8 sequence]")
            if eve_attempt == message_bits:
                print("\n\33[101mEve successfully decrypted the message!\33[0m")
                eveSucceeded = True
                break

        # Step 5: Legitimate Receiver's Decryption
        print("\n\33[32mLegitimate receiver's\33[0m decryption:")
        decrypted_message = apply_one_time_pad(encrypted_message, key)
        decrypted_text = bits_to_string(decrypted_message)
        print(f"Decrypted message:                \33[1;38;5;82m{decrypted_text}\33[0m")

        if decrypted_text != message:
            print("Warning: Decryption verification failed!")
        else:
            print("\33[102mDecryption successful!\33[0m")

        # # Step 6: Optional: Eavesdropping Detection Statistics
        if args.eavesdrop:
            experiment_eavesdropping_detection(args.attempts, required_bits)

    except Exception as e:
        print(f"Error occurred: {str(e)}")
