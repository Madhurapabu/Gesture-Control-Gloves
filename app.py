import keyboard
import time
import serial

sm = serial.Serial(port='COM4', baudrate=38400, timeout=1)


def doAction(action: str) -> None:
    if action == "up":
        keyboard.press_and_release('page up')
    elif action == "down":
        keyboard.press_and_release('page down')
    elif action == "left":
        keyboard.press_and_release("volume up")
    elif action == "right":
        keyboard.press_and_release('volume down')
    else:
        pass


def readArduino() -> None:
    while True:
        try:
            # read and parse serial output
            line = sm.readline().strip().decode(encoding='ASCII')

            # if line is empty skip
            if len(line) == 0:
                continue

            # gesture outputs are in "gesture-xxx" format
            if line.startswith('gesture'):
                gesture = line.split("-")[1]
                print(gesture)
                doAction(gesture)
            else:
                print(line)

        except:
            continue

        finally:
            time.sleep(0.2)


if __name__ == '__main__':
    readArduino()
