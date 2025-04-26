from abc import ABCMeta, abstractmethod
from contextlib import contextmanager

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
import numpy as np

KET_0 = np.array([
    [1],
    [0]
], dtype=complex)
H = np.array([
    [1, 1],
    [1, -1]
], dtype=complex) / np.sqrt(2)
X = np.array([
    [0, 1],
    [1, 0]
], dtype=complex)

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
    available_qubits = [SimulatedQubit()]

    def allocate_qubit(self) -> SimulatedQubit:
        if self.available_qubits:
            return self.available_qubits.pop()

    def deallocate_qubit(self, qubit: SimulatedQubit):
        self.available_qubits.append(qubit)

def prepare_classical_message(bit: bool, q: Qubit) -> None:
    if bit:
        q.x()

def eve_measure(q: Qubit) -> bool:
    return q.measure()

def send_classical_bit(device: QuantumDevice, bit: bool) -> None:
    with device.using_qubit() as q:
        prepare_classical_message(bit, q)
        result = eve_measure(q)
        q.reset()
    assert result == bit


def qrng(device: QuantumDevice) -> bool:
    with device.using_qubit() as q:
        q.h()
        return q.measure()

def prepare_classical_message_plusminus(bit: bool, q: Qubit) -> None:
    if bit:
        q.x()
    q.h()

def eve_measure_plusminus(q: Qubit) -> bool:
    q.h()
    return q.measure()

def send_classical_bit_plusminus(device: QuantumDevice, bit: bool) -> None:
    with device.using_qubit() as q:
        prepare_classical_message_plusminus(bit, q)
        result = eve_measure_plusminus(q)
        assert result == bit

def send_classical_bit_wrong_basis(device: QuantumDevice, bit: bool) -> None:
    with device.using_qubit() as q:
        prepare_classical_message(bit, q)
        result = eve_measure_plusminus(q)
        assert result == bit, "Two parties must (but don't) have the same bit value"

from typing import List

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
    return hex(int(
        "".join(["1" if bit else "0" for bit in bits]),
        2
    ))

def send_single_bit_with_bb84(
    your_device: QuantumDevice,
    eve_device: QuantumDevice
    ) -> tuple:

    [your_message, your_basis] = [
        sample_random_bit(your_device) for _ in range(2)
    ]

    eve_basis = sample_random_bit(eve_device)

    with your_device.using_qubit() as q:
        prepare_message_qubit(your_message, your_basis, q)

        # QUBIT SENDING...

        eve_result = measure_message_qubit(eve_basis, q)

    return ((your_message, your_basis), (eve_result, eve_basis))

def simulate_bb84(n_bits: int, keyFor, attm=1) -> list:
    your_device = SingleQubitSimulator()
    eve_device = SingleQubitSimulator()

    key = []
    n_rounds = 0

    while len(key) < n_bits:
        n_rounds += 1
        ((your_message, your_basis), (eve_result, eve_basis)) = \
            send_single_bit_with_bb84(your_device, eve_device)

        if your_basis == eve_basis:
            assert your_message == eve_result
            key.append(your_message)
    if attm > 20:
        print(f"Took \33[48;5;{n_rounds%216}m\33[38;5;{(n_rounds+15)%216}m{n_rounds}\33[0m rounds to generate a {n_bits}-bit key\
 for \33[48;5;{n_rounds%216}m\33[38;5;{(n_rounds+15)%216}m{keyFor}\33[0m.")

    return key

def apply_one_time_pad(message: List[bool], key: List[bool]) -> List[bool]:
    return [
        message_bit ^ key_bit
        for (message_bit, key_bit) in zip(message, key)
    ]

def string_to_bits(message: str) -> List[bool]:
    """Convert a string message to bits"""
    return [bit == '1' for byte in message.encode('utf-8') 
            for bit in format(byte, '08b')]

def bits_to_string(bits: List[bool]) -> str:
    """Convert bits back to string"""
    bytes_data = []
    for i in range(0, len(bits), 8):
        byte = bits[i:i+8]
        if len(byte) == 8:
            byte_val = int(''.join('1' if b else '0' for b in byte), 2)
            bytes_data.append(byte_val)
    return bytes(bytes_data).decode('utf-8', errors='replace')

def printUsage():
  usage = """
Usage: \33[1mpython bb84_sim.py\33[0m
Optional arguments:
\33[93m  -n <decryption attempt count> \33[2m: [Max] number of times Eve should try to decrypt the message
    (stops at first correct decryption).
  \33[0;93m-m <Message> \33[2m: The message to be sent. (If not provided, you will be prompted to enter it.)
\33[0m
"""
  print(usage)

import argparse
parser = argparse.ArgumentParser(description="BB84 Quantum Key Distribution Simulation")
parser.add_argument("-n", "--attempts", type=int, default=1, help="Number of decryption attempts by Eve")
parser.add_argument("-m", "--message", type=str, help="Message to be sent")
try:
    args = parser.parse_args()
except:
    printUsage()
    exit(1)
if args.attempts < 1:
    print("Error: Number of decryption attempts must be at least 1.")
    exit(1)
if __name__ == "__main__":
    try:
        if args.message:
            message = args.message
            print(f"Using provided secret message: \33[1;38;5;82m{message}\33[0m")
        # Get message from user
        else:
            message = input("Enter your secret message: \33[1;38;5;82m")
            print("\33[0m")
        message_bits = string_to_bits(message)
        required_bits = len(message_bits)
        
        print(f"\33[48;5;18;38;5;33mGenerating a {required_bits}-bit key by simulating BB84...\33[0m")
        key = simulate_bb84(required_bits,"Bob")
        print(f"Got key:                          {convert_to_hex(key)}")
        print(f"Original message bits:            {convert_to_hex(message_bits)}")
        
        encrypted_message = apply_one_time_pad(message_bits, key)
        print(f"Encrypted message:                \33[1m\33[38;5;33m{convert_to_hex(encrypted_message)}\n")
        attempts = 1
        if args.attempts and args.attempts > 1:
            attempts = args.attempts
        eveSucceeded = False
        # Simulate Eve's attempts to decrypt with different random keys
        for i in range(attempts):
            eve_key = simulate_bb84(required_bits,"Eve")  # Eve generates her own key
            eve_attempt = apply_one_time_pad(encrypted_message, eve_key)
            print(f"\33[31mEve's\33[0m decryption attempt \33[48;5;{i%255}m\33[38;5;{(i+15)%255}m{i+1}\33[0m:")
            if attempts<20:
                print(f"Eve's random key:                 {convert_to_hex(eve_key)}")
                print(f"Eve's decrypted bits:            {convert_to_hex(eve_attempt)}")
            try:
                eve_text = bits_to_string(eve_attempt)
                print(f"Eve's decrypted text:            \33[1m\33[38;5;124m{eve_text}\33[0m")
            except:
                print("Eve's decrypted text:            [Invalid UTF-8 sequence]")
            if eve_attempt == message_bits:
                print("\n\33[101mEve successfully decrypted the message!\33[0m")
                eveSucceeded = True
                break
        
        print("\n\33[32mLegitimate receiver's\33[0m decryption:")
        decrypted_message = apply_one_time_pad(encrypted_message, key)
        decrypted_text = bits_to_string(decrypted_message)
        print(f"Decrypted message:                \33[1m\33[38;5;82m{decrypted_text}\33[0m")
        
        if decrypted_text != message:
            print("Warning: Decryption verification failed!")
        else:
            print("\33[102mDecryption successful!\33[0m")
            
    except Exception as e:
        print(f"Error occurred: {str(e)}")