import processing.serial.*;

Serial ComPort;
String input[];

void setup() {
  print(Serial.list());
  String[] devices = Serial.list();
  String arduino = "";
  for(int i = 0; i < devices.length; i++) {
      if(devices[i].equals("/dev/tty.usbmodem1411")) {
        arduino = devices[i];
        break;
      }
  }
  
  if(arduino.equals("")) {
    System.out.println("Could not find arduino");
    //System.exit(1);
  }
  ComPort = new Serial(this, arduino, 9600);
  ComPort.bufferUntil('\n');
}

void draw() {
  input = loadStrings("http://strangerthingsapi.coolcatgames.com/db.py/dequeue");
  if(input != null && input.length != 0) {
    String queue = input[0];
    if(queue.length() != 0) {
      println(queue);
      ComPort.write(queue);
      delay(60000);
    }
  }
  delay(1000);
}