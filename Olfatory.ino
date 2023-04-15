#include <DynamixelSerial.h>
#include <Wire.h>

// units_per_1_degree = 0.29
// max_angle = 0-300 degree (0-1024)
// rot_angle = 60 for example*
int x = 0;
bool FLAG_NEXT_ITERATION = 0;
bool FLAG_STOP_ITERATION = 0;
bool flag_1 = 0;
bool flag_2 = 0;
bool flag_3 = 0;
bool flag_4 = 0;
bool flag_5 = 0;
bool flag_6 = 0;
bool flag_7 = 0;
bool flag_8 = 0;
bool flag_9 = 0;
bool flag_10 = 0;
bool flag_11 = 0;
bool flag_12 = 0;
bool request_action=0;
bool INTERRUT_FLAG = 0;
bool CHECK_FLAG = 0;
bool flag_del = 0;
bool flag_del_two = 0;
bool flag_for_one_marker = 0;
bool is_up = 0;
int z=0;
int speed_of_12 = 100;
String message = "";
char incomingChar;
String marker="";
int last_marker_angle= -666;
int special_angle=0;
int flag_piy_piy = 0;

void position_return (void) // ОТЛАЖЕНО. ТРОЙНАЯ ПРОВЕРКА
{

  int pos_18 = Dynamixel.readPosition (18);
  delay(10);
  if ((pos_18 > 920) | (pos_18 < 890))   // CHECK 1
  {
    Dynamixel.moveSpeed(18, 910, 400);
    delay (2000); // СКОРРЕКТИРОВАТЬ ПОЗЖЕ!
    int pos_18 = Dynamixel.readPosition (18);
    delay(10);
    if ((pos_18 > 920) | (pos_18 < 890))  // CHECK 2
    {
      Dynamixel.moveSpeed(18, 910, 400);
      delay (2000); // СКОРРЕКТИРОВАТЬ ПОЗЖЕ!
      int pos_18 = Dynamixel.readPosition (18);
      delay(10);

      while ((pos_18 > 920) | (pos_18 < 890)) // CHECK 3
      {
        Dynamixel.moveSpeed(18, 910, 400);
        delay (3000);
        pos_18 = Dynamixel.readPosition (18);
        delay(10);
        if (millis() > 15000) // Если двигатели не отвечают или сломались, мы залетаем в бесконечный цикл и запускаем аварийный зуммер.
        {
          pinMode(22, OUTPUT);
         // pinMode(24, OUTPUT);
          while (1)
          {
            digitalWrite(22, ON);
            delay(200);
            digitalWrite(22, OFF);
            delay(200);
          }
        }
      }
    }
  }

  int pos_12 = Dynamixel.readPosition (12);
  delay(10);
  if ((pos_12 > 2) | (pos_12 < 0) )         // CHECK 1
  {
    Dynamixel.moveSpeed(12, 0, 150);
    delay (6000); // СКОРРЕКТИРОВАТЬ ПОЗЖЕ!
    pos_12 = Dynamixel.readPosition (12);
    delay(10);
    if ((pos_12 > 2) | (pos_12 < 0) )         // CHECK 2
    {
      Dynamixel.moveSpeed(12, 0, 150);
      delay (6000); // СКОРРЕКТИРОВАТЬ ПОЗЖЕ!
      pos_12 = Dynamixel.readPosition (12);
      delay(10);
      while ((pos_12 > 2) | (pos_12 < 0))       // CHECK 3
      {
        Dynamixel.moveSpeed(12, 0, 150);
        delay (6000);
        pos_12 = Dynamixel.readPosition (12);
        delay(10);
        if (millis() > 15000)
        {
          pinMode(22, OUTPUT);
          pinMode(24, OUTPUT);
          while (1)
          {
            digitalWrite(22, ON);
            delay(100);
            digitalWrite(22, OFF);
            delay(200);
          }
        }
      }
    }
  }
}

void position_for_18_down (void) // ОТЛАЖЕНО. ТРОЙНАЯ ПРОВЕРКА
{
  int pos_18 = Dynamixel.readPosition (18);
  delay(10);
  if ((pos_18 > 920) | (pos_18 < 890))              // CHECK 1
  {
    Dynamixel.moveSpeed(18, 910, 400);
    delay (2000); // СКОРРЕКТИРОВАТЬ ПОЗЖЕ!
    int pos_18 = Dynamixel.readPosition (18);
    delay(10);
    if ((pos_18 > 920) | (pos_18 < 890))             // CHECK 2
    {
      Dynamixel.moveSpeed(18, 910, 400);
      delay (2000); // СКОРРЕКТИРОВАТЬ ПОЗЖЕ!

      Dynamixel.moveSpeed(18, 500, 400);
      delay (5000);

      Dynamixel.moveSpeed(18, 920, 400);
      delay (5000);

      pos_18 = Dynamixel.readPosition (18);
      delay(10);
      if ((pos_18 > 920) | (pos_18 < 890))             // CHECK 3
      {
        Dynamixel.moveSpeed(18, 910, 400);
        delay (2000); // СКОРРЕКТИРОВАТЬ ПОЗЖЕ!
        int pos_18 = Dynamixel.readPosition (18);
        delay(10);

        if  ((pos_18 > 920) | (pos_18 < 890))
        {
          pinMode(22, OUTPUT);
          pinMode(24, OUTPUT);
          while (1) // БЛОКИРОВАНИЕ
          {
            digitalWrite(22, ON);
            delay(200);
            digitalWrite(22, OFF);
            delay(200);
          }
        }
      }
    }
  }
}

void wait_for_position_ID_12 (int input_position)
{
  int pos = Dynamixel.readPosition(12);
  delay(10);
  int input_position_plus_three = pos + 3;
  int input_position_minus_three = pos - 3;
  int x = 0;
  int y = 0;
  
  while ((input_position < input_position_minus_three) | (input_position > input_position_plus_three))
  {
    delay(200);
    pos = Dynamixel.readPosition(12);
    delay(10);
    input_position_plus_three = pos + 3;
    input_position_minus_three = pos - 3;
    x = x+1;
    y = y+1;
    if (x>35)
    {
      Dynamixel.moveSpeed(12, 0, 100);
      delay (6000);
      Dynamixel.moveSpeed(12, input_position, 100);
      delay (6000);
      x = 0;
    }
    
    if (y>45) 
    {
       while (1) // БЛОКИРОВАНИЕ
       {
         digitalWrite(22, ON);
         delay(200);
         digitalWrite(22, OFF);
         delay(200);
       }
    }
  }
  delay(600);
  x=0;
  y=0;
}

void check_position_ID_12 (int input_position) // ОТЛАЖЕНО. ТРОЙНАЯ ПРОВЕРКА
{
  int pos = Dynamixel.readPosition(12);
  delay(10);
  int input_position_plus_three = pos + 3;
  int input_position_minus_three = pos - 3;
  if (input_position == 0) {
    input_position_minus_three = -6666;
  }
  if (input_position == 1023) {
    input_position_plus_three = 6666;
  }

  if ((pos > (input_position_plus_three)) | (pos < (input_position_minus_three)) | (pos < 0))  // CHECK 1
  {
    Dynamixel.moveSpeed(12, input_position, 100);
    delay (5000);
    pos = Dynamixel.readPosition(12);
    delay(10);
    if ((pos > (input_position_plus_three)) | (pos < (input_position_minus_three)) | (pos < 0))  // CHECK 2
    {
      Dynamixel.moveSpeed(12, 500, 100);
      delay (5000);
      Dynamixel.moveSpeed(12, input_position, 100);
      delay (5000);
      pos = Dynamixel.readPosition(12);
      delay(10);

      if ((pos > (input_position_plus_three)) | (pos < (input_position_minus_three)) | (pos < 0))  // CHECK 3
      {
        Dynamixel.moveSpeed(12, input_position, 100);
        delay (5000);
        pos = Dynamixel.readPosition(12);
        delay(10);
        if ((pos > (input_position_plus_three)) | (pos < (input_position_minus_three)) | (pos < 0))
        {
          pinMode(22, OUTPUT);
          pinMode(24, OUTPUT);
          while (1) // БЛОКИРОВАНИЕ
          {
            digitalWrite(22, ON);
            delay(200);
            digitalWrite(22, OFF);
            delay(200);
          }
        }
      }
    }
  }
}

void check_position_ID_18 (void) // ОТЛАЖЕНО. ТРОЙНАЯ ПРОВЕРКА
{
  int pos = Dynamixel.readPosition (18);
  delay(10);
  if ((pos < 130 | (pos < 0)))              // CHECK 1
  {
    Dynamixel.moveSpeed(18, 144, 400);
    delay (5000);
    pos = Dynamixel.readPosition (18);
    delay(10);

    if ((pos < 130) | (pos < 0))                  // CHECK 2
    {
      Dynamixel.moveSpeed(18, 500, 400);
      delay (5000);
      Dynamixel.moveSpeed(18, 144, 400);
      delay (5000);
      pos = Dynamixel.readPosition (18);
      delay(10);

      if ((pos < 130) | (pos < 0))                  // CHECK 3
      {
        Dynamixel.moveSpeed(18, 144, 400);
        delay (5000);
        pos = Dynamixel.readPosition (18);
        delay(10);

        if ((pos < 130) | (pos < 0))
        {
          pinMode(22, OUTPUT);
          pinMode(24, OUTPUT);
          while (1) // БЛОКИРОВАНИЕ
          {
            digitalWrite(22, ON);
            delay(200);
            digitalWrite(22, OFF);
            delay(200);
          }
        }
      }
    }
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length();

    for (int i = 0; i <= maxIndex && found <= index; i++) 
    {
        if (data.charAt(i) == separator || i == maxIndex) 
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void setup() {
  Dynamixel.setSerial(&Serial1);
  Dynamixel.begin(1000000, 28);
  randomSeed(analogRead(A7));
  delay(200);
  Dynamixel.setCMargin (12, 0, 0);
  delay(200);
  Dynamixel.setCMargin (18, 0, 0);
  delay(200);
  pinMode(21, INPUT_PULLUP);   
  while(digitalRead(21)==1) // Ждет сигнала с оптопары что устройство включили кнопкой и динамиксели запитаны. Иначе начинают орать зумеры после установления отсутствия связи с динамикселями.
  {1;}
  position_return();
  delay(500); // Чтобы устаканились колебания
  pinMode(9, OUTPUT);
  digitalWrite(9,ON);
}

void loop() {

  if(digitalRead(21)==1) // Ждет сигнала с оптопары что устройство включили кнопкой и динамиксели запитаны. Иначе начинают орать зумеры после установления отсутствия связи с динамикселями.
  {digitalWrite(9,OFF);}
  while(digitalRead(21)==1) // Ждет сигнала с оптопары что устройство включили кнопкой и динамиксели запитаны. Иначе начинают орать зумеры после установления отсутствия связи с динамикселями.
  {1;}
  if(digitalRead(21)!=1) // Ждет сигнала с оптопары что устройство включили кнопкой и динамиксели запитаны. Иначе начинают орать зумеры после установления отсутствия связи с динамикселями.
  {digitalWrite(9,ON);}
  
  Dynamixel.end();                 // End Servo Comunication
  delay(10);
  Serial.begin(9600);              // Begin Serial Comunication
  delay(10);
  
  if (Serial.available() > 0)
  {
    marker = Serial.readStringUntil('\n');

    Serial.println(marker);

    if (marker != ""){request_action = 1;}
    
    Serial.end();                     // End the Serial Comunication
    delay(30);
    Dynamixel.begin(1000000, 28);     // Begin Servo Comunication
    delay(30);
  }
    
    int pos_marker=0;   

    if      (marker == "1") {pos_marker =   0;  special_angle = 666;}
    else if (marker == "2") {pos_marker =  88;  special_angle =   0;}
    else if (marker == "3") {pos_marker = 180;  special_angle =  88;}
    else if (marker == "4") {pos_marker = 275;  special_angle = 180;}
    else if (marker == "5") {pos_marker = 371;  special_angle = 275;}
    else if (marker == "6") {pos_marker = 464;  special_angle = 371;}
    else if (marker == "7") {pos_marker = 556;  special_angle = 464;}
    else if (marker == "8") {pos_marker = 648;  special_angle = 556;}
    else if (marker == "9") {pos_marker = 742;  special_angle = 648;}
    else if (marker == "10") {pos_marker = 838;  special_angle = 742;}
    else if (marker == "11") {pos_marker = 932;  special_angle = 838;}
    else if (marker == "12") {pos_marker = 1023; special_angle = 932; flag_piy_piy=1;}
    else if (marker == "stop") {position_return(); marker = ""; request_action = 0; last_marker_angle= -666; special_angle = 666;}

    if (marker=="" && request_action==1) {request_action = 0;}
    
    if (request_action == 1) 
    {

      if (last_marker_angle > pos_marker)
     {
      
      if (special_angle!=666)
      {
        position_for_18_down ();
        Dynamixel.moveSpeed(12, special_angle, speed_of_12);
        wait_for_position_ID_12 (special_angle);
        special_angle = 666;
      }
     }
      position_for_18_down ();
      Dynamixel.moveSpeed(12, pos_marker, speed_of_12);
      wait_for_position_ID_12 (pos_marker);
      Dynamixel.moveSpeed(18, 144, 250);
      delay(2500);
      check_position_ID_18();
      request_action = 0; 
      marker = "";
      last_marker_angle = pos_marker;
      pos_marker = 0;
      delay(2000 + 5000);
      position_for_18_down ();
      if (flag_piy_piy == 1) {position_return(); flag_piy_piy=0;} // возвращает маркер назад
    }
    }
