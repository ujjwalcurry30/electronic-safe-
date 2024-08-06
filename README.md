# Face Authentication with Electronic Safe

This project uses face recognition to authenticate a user and then interact with an electronic safe simulator.

## Getting Started

1. Navigate to the [`faceAuthentication`](command:_github.copilot.openRelativePath?%5B%22faceAuthentication%22%5D "faceAuthentication") directory

```sh
cd faceAuthentication
```

2. Run the [`main.py`](command:_github.copilot.openRelativePath?%5B%22faceAuthentication%2Fmain.py%22%5D "faceAuthentication/main.py") script:

```sh
python main.py
```

This will open your camera and start the face recognition process. Once your face is recognized, the script will automatically open the Wokwi simulator.

## Wokwi Simulator

The Wokwi simulator is located at [this link](https://wokwi.com/projects/387337896026447873). The files for the simulator are stored in the [`electronic-safe`](command:_github.copilot.openRelativePath?%5B%22electronic-safe%22%5D "electronic-safe") directory of this project.

In the simulator, the electronic safe will first ask for a secret code. It will then suggest a password and ask you to enter a password. The safe will tell you the strength of the password you entered. Once the safe is locked, it will ask for the same password to unlock it. If you enter the wrong password three times, the safe will terminate.

## Project Structure

- [`faceAuthentication/`](command:_github.copilot.openRelativePath?%5B%22faceAuthentication%2F%22%5D "faceAuthentication/"): Contains the main python file to be executed
- [`electronic-safe/`](command:_github.copilot.openRelativePath?%5B%22electronic-safe%2F%22%5D "electronic-safe/"): Contains the arduino files for the Wokwi simulator.
- [`model/`](command:_github.copilot.openRelativePath?%5B%22model%2F%22%5D "model/"): Contains the datasets, training & face recognition model.

## Contributers
- 2021UCS1618 **Shobhit**
- 2021UCS1702 **Prachi Sah**
- 2021UCS1659 **Ujjwal**
