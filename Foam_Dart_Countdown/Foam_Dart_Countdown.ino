// Define the LED digit patters, from 0 to 9
// Note that these patterns are for common anode displays
// 0 = LED on, 1 = LED off:


int displayCount = 25;  // Intial count

int countSet = displayCount; // Store initial count

const int analogInPin = A2;  // Analog input pin that the ir reciever is attached to

int sensorValue = 0;        // value read from the ir beam
int outputValue = 0;        // value output to the PWM (analog out)
boolean hasCleared = true;

int firstDigit;
int secondDigit;
boolean resetState; 

const int resetPin = 6;     // Use digital pin 6 for the reset pin

int SER_Pin = 7;   //pin 14 on the 75HC595
int RCLK_Pin = 8;  //pin 12 on the 75HC595
int SRCLK_Pin = 9; //pin 11 on the 75HC595

//How many of the shift registers - change this
#define number_of_74hc595s 2 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

void setup() {

  pinMode(resetPin, INPUT);
  digitalWrite(resetPin, HIGH);
  
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);

  Serial.begin(9600);
  
  //reset all register pins
  // clearRegisters();
  // writeRegisters();
  
  changeNumber();
  
}               

void loop(){
  
  sensorValue = analogRead(analogInPin); // read the analog in value:
  outputValue = map(sensorValue, 0, 1023, 0, 255);  // map it to the range of the analog out:
  
  resetState = digitalRead(resetPin);
  
  // print the results to the serial monitor:
  
  //Serial.print("sensor = " );                       
  //Serial.print(sensorValue);      
  if (outputValue > 100) {
    Serial.print("\t output = ");      
    Serial.println(outputValue);
  }
  
  // if dart is cleared and beam is broken then countdown
  if ( hasCleared == true ) {
  
    if (outputValue > 200) {       
      changeNumber(); 
      hasCleared = false;
      //delay(2);
    }
  }
  
  // check to see if dart has cleared
  if (outputValue < 50) {
    hasCleared = true;
  }
  
  // check if the magazine has been ejected and reloaded
  if (resetState == LOW) {       
    resetNumber(); 
  }
  

}

void resetNumber() {
  
  displayCount = countSet - 1;
  
  displayNumber(2,5);
  writeRegisters();  //Send data to the 75HC595
}

void changeNumber() {

  if( (displayCount < 100) && (displayCount > 9) ) {
    
    firstDigit = displayCount / 10; // Find the first digit
    secondDigit = displayCount % 10; 
    
    displayNumber(firstDigit,secondDigit); //Display the digits
    
    // Serial.println(secondDigit);
    
    displayCount--;
    
    writeRegisters();  //Send data to the 75HC595
  }

  else if( (displayCount < 10) && (displayCount > 0) ) {
    
    firstDigit = 0; // Set the first digit to 0
    secondDigit = displayCount;
    
    displayNumber(firstDigit,secondDigit); //Display the section
    displayCount--;
    
    writeRegisters();
  
  }
  
  else if( displayCount == 0) {
    
    firstDigit = 0; // Set the first digit to 0
    secondDigit = 0; // Set the second digit to 0
    
    displayNumber(firstDigit,secondDigit); //Display the section
    
    // displayNumber(10); //Turn off all segments
    
    // add double zero blink here
    
    
    // clearRegisters();
    writeRegisters(); 
  }
  
  delay(250); // Set iteration for 1 second  
  
  //Serial.println(displayCount);
  
  
}


void displayNumber(int digit1, int digit2) {

  for (int digit = numOfRegisterPins ; digit > 0 ; digit--) {
    
    //Turn on the right segments for this digit
    firstNumber(digit1);
    secondNumber(digit2);
    
  }

}

//Given a number, turns on those segments
//If number == 10, then turn off number
void firstNumber(int numberToDisplay) {
  
  switch (numberToDisplay){
    
  // Display number 0
  case 0:
    setRegisterPin(0, LOW);
    setRegisterPin(1, LOW);
    setRegisterPin(2, LOW);
    setRegisterPin(3, LOW);
    setRegisterPin(4, LOW);
    setRegisterPin(5, LOW);
    setRegisterPin(6, HIGH);
    break;
  
  // Display number 1
  case 1:
    setRegisterPin(0, HIGH);
    setRegisterPin(1, LOW);
    setRegisterPin(2, LOW);
    setRegisterPin(3, HIGH);
    setRegisterPin(4, HIGH);
    setRegisterPin(5, HIGH);
    setRegisterPin(6, HIGH);
    break; 
  
  // Display number 2
  case 2:
    setRegisterPin(0, LOW);
    setRegisterPin(1, LOW);
    setRegisterPin(2, HIGH);
    setRegisterPin(3, LOW);
    setRegisterPin(4, LOW);
    setRegisterPin(5, HIGH);
    setRegisterPin(6, LOW);
    break;
  
  // Display number 3
  case 3:
    setRegisterPin(0, LOW);
    setRegisterPin(1, LOW);
    setRegisterPin(2, LOW);
    setRegisterPin(3, LOW);
    setRegisterPin(4, HIGH);
    setRegisterPin(5, HIGH);
    setRegisterPin(6, LOW);
    break;   

  // Display number 4
  case 4:
    setRegisterPin(0, HIGH);
    setRegisterPin(1, LOW);
    setRegisterPin(2, LOW);
    setRegisterPin(3, HIGH);
    setRegisterPin(4, HIGH);
    setRegisterPin(5, LOW);
    setRegisterPin(6, LOW);
    break;

  // Display number 5
  case 5:
    setRegisterPin(0, LOW);
    setRegisterPin(1, HIGH);
    setRegisterPin(2, LOW);
    setRegisterPin(3, LOW);
    setRegisterPin(4, HIGH);
    setRegisterPin(5, LOW);
    setRegisterPin(6, LOW);
    break;
  
  // Display number 6
  case 6:
    setRegisterPin(0, LOW);
    setRegisterPin(1, HIGH);
    setRegisterPin(2, LOW);
    setRegisterPin(3, LOW);
    setRegisterPin(4, LOW);
    setRegisterPin(5, LOW);
    setRegisterPin(6, LOW);
    break;

  // Display number 7
  case 7:
    setRegisterPin(0, LOW);
    setRegisterPin(1, LOW);
    setRegisterPin(2, LOW);
    setRegisterPin(3, HIGH);
    setRegisterPin(4, HIGH);
    setRegisterPin(5, HIGH);
    setRegisterPin(6, HIGH);
    break;

  // Display number 8
  case 8:
    setRegisterPin(0, LOW);
    setRegisterPin(1, LOW);
    setRegisterPin(2, LOW);
    setRegisterPin(3, LOW);
    setRegisterPin(4, LOW);
    setRegisterPin(5, LOW);
    setRegisterPin(6, LOW);
    break;
  
  // Display number 9
  case 9:
    setRegisterPin(0, LOW);
    setRegisterPin(1, LOW);
    setRegisterPin(2, LOW);
    setRegisterPin(3, HIGH);
    setRegisterPin(4, HIGH);
    setRegisterPin(5, LOW);
    setRegisterPin(6, LOW);
    break;
    
  // Turn off display
  case 10:
    setRegisterPin(0, HIGH);
    setRegisterPin(1, HIGH);
    setRegisterPin(2, HIGH);
    setRegisterPin(3, HIGH);
    setRegisterPin(4, HIGH);
    setRegisterPin(5, HIGH);
    setRegisterPin(6, HIGH);
    break;
  
  }
}

//Given a number, turns on those segments
//If number == 10, then turn off number
void secondNumber(int numberToDisplay) {
  
  switch (numberToDisplay){
    
  // Display number 0
  case 0:
    setRegisterPin(8, LOW);
    setRegisterPin(9, LOW);
    setRegisterPin(10, LOW);
    setRegisterPin(11, LOW);
    setRegisterPin(12, LOW);
    setRegisterPin(13, LOW);
    setRegisterPin(14, HIGH);
    break;
  
  // Display number 1
  case 1:
    setRegisterPin(8, HIGH);
    setRegisterPin(9, LOW);
    setRegisterPin(10, LOW);
    setRegisterPin(11, HIGH);
    setRegisterPin(12, HIGH);
    setRegisterPin(13, HIGH);
    setRegisterPin(14, HIGH);
    break; 
  
  // Display number 2
  case 2:
    setRegisterPin(8, LOW);
    setRegisterPin(9, LOW);
    setRegisterPin(10, HIGH);
    setRegisterPin(11, LOW);
    setRegisterPin(12, LOW);
    setRegisterPin(13, HIGH);
    setRegisterPin(14, LOW);
    break;
  
  // Display number 3
  case 3:
    setRegisterPin(8, LOW);
    setRegisterPin(9, LOW);
    setRegisterPin(10, LOW);
    setRegisterPin(11, LOW);
    setRegisterPin(12, HIGH);
    setRegisterPin(13, HIGH);
    setRegisterPin(14, LOW);
    break;   

  // Display number 4
  case 4:
    setRegisterPin(8, HIGH);
    setRegisterPin(9, LOW);
    setRegisterPin(10, LOW);
    setRegisterPin(11, HIGH);
    setRegisterPin(12, HIGH);
    setRegisterPin(13, LOW);
    setRegisterPin(14, LOW);
    break;

  // Display number 5
  case 5:
    setRegisterPin(8, LOW);
    setRegisterPin(9, HIGH);
    setRegisterPin(10, LOW);
    setRegisterPin(11, LOW);
    setRegisterPin(12, HIGH);
    setRegisterPin(13, LOW);
    setRegisterPin(14, LOW);
    break;
  
  // Display number 6
  case 6:
    setRegisterPin(8, LOW);
    setRegisterPin(9, HIGH);
    setRegisterPin(10, LOW);
    setRegisterPin(11, LOW);
    setRegisterPin(12, LOW);
    setRegisterPin(13, LOW);
    setRegisterPin(14, LOW);
    break;

  // Display number 7
  case 7:
    setRegisterPin(8, LOW);
    setRegisterPin(9, LOW);
    setRegisterPin(10, LOW);
    setRegisterPin(11, HIGH);
    setRegisterPin(12, HIGH);
    setRegisterPin(13, HIGH);
    setRegisterPin(14, HIGH);
    break;

  // Display number 8
  case 8:
    setRegisterPin(8, LOW);
    setRegisterPin(9, LOW);
    setRegisterPin(10, LOW);
    setRegisterPin(11, LOW);
    setRegisterPin(12, LOW);
    setRegisterPin(13, LOW);
    setRegisterPin(14, LOW);
    break;
  
  // Display number 9
  case 9:
    setRegisterPin(8, LOW);
    setRegisterPin(9, LOW);
    setRegisterPin(10, LOW);
    setRegisterPin(11, HIGH);
    setRegisterPin(12, HIGH);
    setRegisterPin(13, LOW);
    setRegisterPin(14, LOW);
    break;
    
  // Turn off display
  case 10:
    setRegisterPin(8, HIGH);
    setRegisterPin(9, HIGH);
    setRegisterPin(10, HIGH);
    setRegisterPin(11, HIGH);
    setRegisterPin(12, HIGH);
    setRegisterPin(13, HIGH);
    setRegisterPin(14, HIGH);
    break;
  
  }
}

//set all register pins to HIGH
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = HIGH;
  }
} 


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for (int i = numOfRegisterPins - 1; i >=  0; i--) {
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];
    
    // Serial.println(registers[i]);

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}


