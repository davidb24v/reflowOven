void button1Press() {
  button1Pressed = true;
}
void button2Press() {
  button2Pressed = true;
}
void button3Press() {
  button3Pressed = true;
}

void buttonSetup() {
  PCintPort::attachInterrupt(Button1, &button1Press, FALLING);
  PCintPort::attachInterrupt(Button2, &button2Press, FALLING);
  PCintPort::attachInterrupt(Button3, &button3Press, FALLING);
}

void clearAllButtons() {
  button1Pressed = false;
  button2Pressed = false;
  button3Pressed = false;
}

