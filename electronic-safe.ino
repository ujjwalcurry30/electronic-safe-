#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include "SafeState.h"
#include "icons.h"


#define SERVO_PIN 6
#define SERVO_LOCK_POS   20
#define SERVO_UNLOCK_POS 90
#define PASSWORD_LENGTH 4

Servo lockServo;

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

int count = 0;

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colPins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

SafeState safeState;

void lock() {
  lockServo.write(SERVO_LOCK_POS);
  safeState.lock();
}

void unlock() {
  lockServo.write(SERVO_UNLOCK_POS);
}

void showStartupMessage() {
  lcd.setCursor(4, 0);
  lcd.print("Welcome!");
  delay(1000);

  lcd.setCursor(0, 2);
  String message = "Electronic Safe";
  for (byte i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(100);
  }
  delay(500);
}

void showWaitScreen(int delayMillis) {
  lcd.setCursor(2, 1);
  lcd.print("[..........]");
  lcd.setCursor(3, 1);
  for (byte i = 0; i < 10; i++) {
    delay(delayMillis);
    lcd.print("=");
  }
}

void showUnlockMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  lcd.setCursor(4, 0);
  lcd.print("Unlocked!");
  lcd.setCursor(15, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  delay(1000);
}


int getPassStrength(String code = "") {
  int strength = 0;
  if (code.length() == 0) {
    code = safeState.getCode();
  }
  if (code.length() == 0) {
    return 0;
  }
  bool digitUsed[10] = {false};
  for (int i = 0; i < code.length(); i++) {
    int digit = code.charAt(i) - '0';
    if (!digitUsed[digit]) {
      digitUsed[digit] = true;
      strength++;
    }
  }
  return strength;
}

String generatePassword() {
  char pass[4];
  String password = "";
  do {
    for (int i = 0; i < PASSWORD_LENGTH; i++) {
      pass[i] = random(0, 10) + '0';
    }
    password = String(pass);
    if (getPassStrength(password) == 4)
      break;
  } while(true);
  return password;
}

String inputSecretCode(int temp=1) {
  if(temp == 0) {
  String suggested = generatePassword();
  lcd.setCursor(0, 1);
  lcd.print("[____] Sug: ");
  lcd.print(suggested);
  lcd.setCursor(1, 1);
  }
  else {
    lcd.setCursor(5, 1);
    lcd.print("[____]");
    lcd.setCursor(6, 1);
  }
  String result = "";
  while (result.length() < 4) {
    char key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      lcd.print('*');
      result += key;
    }
  }
  return result;
}

void printPassStrength() {
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PasswordStrength");
  lcd.setCursor(0, 1);
  lcd.print("is ");
  // lcd.print(getPassStrength());
  int strength = getPassStrength();
  if (strength >= 3) {
    lcd.print(" (Strong)");
  } else if (strength == 2) {
    lcd.print(" (Medium)");
  } else {
    lcd.print(" (Weak)");
  }
  delay(2000);
}

bool setNewCode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter new code:");
  String newCode = inputSecretCode(0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Confirm new code");
  String confirmCode = inputSecretCode();

  if (newCode.equals(confirmCode)) {
    safeState.setCode(newCode);
    printPassStrength();
    return true;
  } else {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Code mismatch");
    lcd.setCursor(0, 1);
    lcd.print("Safe not locked!");
    delay(2000);
    return false;
  }
}

void safeUnlockedLogic() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(ICON_UNLOCKED_CHAR);
  lcd.setCursor(2, 0);
  lcd.print(" # to lock");
  lcd.setCursor(15, 0);
  lcd.write(ICON_UNLOCKED_CHAR);

  bool newCodeNeeded = true;

  if (safeState.hasCode()) {
    lcd.setCursor(0, 1);
    lcd.print("  A = new code");
    newCodeNeeded = false;
  }

  auto key = keypad.getKey();
  while (key != 'A' && key != '#') {
    key = keypad.getKey();
  }

  bool readyToLock = true;
  if (key == 'A' || newCodeNeeded) {
    readyToLock = setNewCode();
  }

  if (readyToLock) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.write(ICON_UNLOCKED_CHAR);
    lcd.print(" ");
    lcd.write(ICON_RIGHT_ARROW);
    lcd.print(" ");
    lcd.write(ICON_LOCKED_CHAR);

    safeState.lock();
    lock();
    showWaitScreen(100);
  }
}

void safeLockedLogic() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(ICON_LOCKED_CHAR);
  lcd.print(" Safe Locked! ");
  lcd.write(ICON_LOCKED_CHAR);

  String userCode = inputSecretCode();
  bool unlockedSuccessfully = safeState.unlock(userCode);
  showWaitScreen(200);

  if (unlockedSuccessfully) {
    showUnlockMessage();
    unlock();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Denied! ");
    lcd.print(3-count);
    lcd.print(" AttemptLeft");
    showWaitScreen(200);
  }
}

int enterSecretKey() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter secret key:");
  lcd.setCursor(5, 1);
  lcd.print("[____]");
  lcd.setCursor(6, 1);

  String result = "";
  while (result.length() < 4) {
    char key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      lcd.print('*');
      result += key;
    }
  }
  if (result.equals("5282")) {
    return 1;
  } else {
    return 0;
  }
}

void showTerminate() {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Wrong Code :)");
    lcd.setCursor(1, 1);
    lcd.print("Terminating!!!");
    while(true);
}

void setup() {
  lcd.begin(16, 2);
  init_icons(lcd);

  lockServo.attach(SERVO_PIN);

  Serial.begin(115200);
  
  long seed = 0;
  for (int i = 0; i < 50; i++) {
    seed += analogRead(i);
  }
  randomSeed(seed);
  
  if (safeState.locked()) {
    lock();
  } else {
    unlock();
  }

  showStartupMessage();

  if (enterSecretKey() == 0)
    showTerminate();
}

void loop() {
  if (safeState.locked()) {
    if (count++ > 2)
      showTerminate();
    safeLockedLogic();
  } else {
    count = 0;
    safeUnlockedLogic();
  }
}
