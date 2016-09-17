#!/usr/bin/env python3
import sys
import time
import glob

import serial
import psutil


CPU_INTERVAL = 0.5
CONNECT_TIMEOUT = 2
BAUD = 4800

def update_loop(conn):
    while True:
        load = psutil.cpu_percent(interval=CPU_INTERVAL)
        scaled_load = int(load * 10)
        message = str(scaled_load).encode('ascii')
        conn.write(message)

def connect_serial():
    devices = glob.glob('/dev/ttyUSB?')

    if not devices:
        devices = glob.glob('/dev/tty.wchusbserial*')

    if not devices:
        raise IOError()

    conn = serial.Serial(devices[0], BAUD)

    # wtf
    conn.baudrate = 300
    conn.baudrate = BAUD

    return conn

def main():
    while True:
        try:
            with connect_serial() as conn:
                update_loop(conn)
        except IOError:
            print('Connection failed! Retrying in %d seconds...'
                % CONNECT_TIMEOUT, file=sys.stderr)
            time.sleep(CONNECT_TIMEOUT)


if __name__ == '__main__':
    main()
