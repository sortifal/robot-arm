#include "FastAccelStepper.h"
#include <iostream>
#include <Arduino.h>
#include <tuple>

// As in StepperDemo for Motor 1 on ESP32
#define enablePinStepper1 21
#define enablePinStepper2 4
#define XYz 287.87
#define XYx 20.174
#define XY sqrt(ABz *ABz + ABx * ABx)
#define YZ 260.986
#define ZBx 19.219
#define ZBy 260.753
#define ZB sqrt(ZBx *ZBx + ZBy * ZBy)
#define AC 74.745
int reduction[] = { 87040, 128000, 128000, 32000, 14880, 3200 };
int x = 6400 * 4;

void function1(void);
void function2(void);
void function3(void);
void processGCodeLine(String);

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepperX = NULL;
FastAccelStepper *stepperY = NULL;
FastAccelStepper *stepperZ = NULL;
FastAccelStepper *stepperA = NULL;
FastAccelStepper *stepperB = NULL;
FastAccelStepper *stepperC = NULL;

struct Sorties {
  int X;
  int Y;
  int Z;
  int A;
  int B;
  int C;
};

void setup() {
  Serial.begin(115200);
  engine.init();
  stepperX = engine.stepperConnectToPin(13);
  if (stepperX) {
    stepperX->setEnablePin(enablePinStepper1);
    stepperX->setDirectionPin(12);
    stepperX->setAutoEnable(false);
    stepperX->setSpeedInUs(30);        // the parameter is us/step !!!//100
    stepperX->setAcceleration(70000);  //10000
    //stepperX->move(1000);
  }
  stepperY = engine.stepperConnectToPin(27);
  if (stepperY) {
    stepperY->setEnablePin(enablePinStepper1);
    stepperY->setDirectionPin(14);
    stepperY->setAutoEnable(false);
    stepperY->setSpeedInUs(30);        // the parameter is us/step !!!//100
    stepperY->setAcceleration(50000);  //10000
    //stepperY->move(-1000);
  }
  stepperZ = engine.stepperConnectToPin(25);
  if (stepperZ) {
    stepperZ->setEnablePin(enablePinStepper1);
    stepperZ->setDirectionPin(26);
    stepperZ->setAutoEnable(false);
    stepperZ->setSpeedInUs(30);         // the parameter is us/step !!!//100
    stepperZ->setAcceleration(100000);  //10000
    //stepperZ->move(-1000);
  }
  stepperA = engine.stepperConnectToPin(32);
  if (stepperA) {
    stepperA->setEnablePin(enablePinStepper2);
    stepperA->setDirectionPin(33);
    stepperA->setAutoEnable(false);
    stepperA->setSpeedInUs(50);         // the parameter is us/step !!!//100
    stepperA->setAcceleration(100000);  //10000
    //stepperA->move(-1000);
  }
  stepperB = engine.stepperConnectToPin(18);
  if (stepperB) {
    stepperB->setEnablePin(enablePinStepper2);
    stepperB->setDirectionPin(19);
    stepperB->setAutoEnable(false);
    stepperB->setSpeedInUs(100);       // the parameter is us/step !!!//100
    stepperB->setAcceleration(10000);  //10000
    //stepperB->move(-1000);
  }
  stepperC = engine.stepperConnectToPin(22);
  if (stepperC) {
    stepperC->setEnablePin(enablePinStepper2);
    stepperC->setDirectionPin(23);
    stepperC->setAutoEnable(false);
    stepperC->setSpeedInUs(100);       // the parameter is us/step !!!//100
    stepperC->setAcceleration(10000);  //10000
    //stepperC->move(-1000);
  } else {
    Serial.printf("error");
  }
  Serial.printf("init finit \n");
  Serial.printf("ATTENTION Y ET Z EN NEGATIF \n");
}

void loop() {
  static String line = "";  // Holds the incoming line

  while (Serial.available() > 0) {
    char receivedChar = Serial.read();  // Read the next character

    if (receivedChar == '\n' || receivedChar == '\r') {
      // End of line character found, process the line
      if (line.length() > 0) {   // Make sure the line is not empty
        processGCodeLine(line);  // Process the line of G-code
        line = "";               // Reset the line buffer for the next line
      }
    } 
    else {
      line += receivedChar;  // Add the character to the line
    }
  }
}
//fonction de test qui tourne entre 3 position
void function1(void) {
  Serial.printf("/n fonction1");
  int it = 0;
  while (it < 2) {
    stepperX->moveTo(20000);
    stepperY->moveTo(-48000);
    stepperZ->moveTo(-38000);
    stepperA->moveTo(-7000);
    stepperB->moveTo(-1700);
    stepperC->moveTo(800);
    while ((stepperX->isRunning()) || (stepperY->isRunning()) || (stepperZ->isRunning()) || (stepperA->isRunning()) || (stepperB->isRunning()) || (stepperC->isRunning())) {}
    stepperX->moveTo(0);
    stepperY->moveTo(-20000);
    stepperZ->moveTo(-9000);
    stepperA->moveTo(0);
    stepperB->moveTo(2000);
    stepperC->moveTo(0);
    while ((stepperX->isRunning()) || (stepperY->isRunning()) || (stepperZ->isRunning()) || (stepperA->isRunning()) || (stepperB->isRunning()) || (stepperC->isRunning())) {}
    stepperX->moveTo(-20000);
    stepperY->moveTo(-48000);
    stepperZ->moveTo(-38000);
    stepperA->moveTo(7000);
    stepperB->moveTo(-3000);
    stepperC->moveTo(-800);
    while ((stepperX->isRunning()) || (stepperY->isRunning()) || (stepperZ->isRunning()) || (stepperA->isRunning()) || (stepperB->isRunning()) || (stepperC->isRunning())) {}
    stepperX->moveTo(0);
    stepperY->moveTo(-20000);
    stepperZ->moveTo(-9000);
    stepperA->moveTo(0);
    stepperB->moveTo(2000);
    stepperC->moveTo(0);
    while ((stepperX->isRunning()) || (stepperY->isRunning()) || (stepperZ->isRunning()) || (stepperA->isRunning()) || (stepperB->isRunning()) || (stepperC->isRunning())) {}
    it += 1;
  }
  stepperX->moveTo(0);
  stepperY->moveTo(-100);
  stepperZ->moveTo(-100);
  stepperA->moveTo(0);
  stepperB->moveTo(0);
  stepperC->moveTo(0);
  Serial.printf("moteur home");
}
/*
void function2(void) {
  Serial.printf("\n x:");
  while (Serial.available() == 0) {}
  float x = Serial.readString().toDouble();
  Serial.printf("\n y:");
  while (Serial.available() == 0) {}
  float y = Serial.readString().toDouble();
  Serial.printf("\n z:");
  while (Serial.available() == 0) {}
  float z = Serial.readString().toDouble();
  CalculMoteurSimple(x, y, z);
}*/
/* c'est de la merde sa je crois
Sorties CalculMoteurSimple(double x, double y, double z) {
  Sorties resultats;
  double X = atan2(y, x) * 180.0 / M_PI;
  double YBx = x - XYx - AC;
  double YBy = z + XYz;
  double YB = sqrt(pow(YBx, 2.00) + pow(YBy, 2.00));
  double Ycomp = 90 - (tan(YBy / YBx));
  double Y = acos(((YB * YB) + (YZ * YZ) - (ZB * ZB)) / (2.00 * YZ * YB)) * 180.0 / M_PI + Ycomp;
  double Z = acos(((ZB * ZB) + (YZ * YZ) - (YB * YB)) / (2.00 * YZ * ZB)) * 180.0 / M_PI;
  double B = 90.00 - Ycomp;
  X = ((X / reduction[0]) / 360);
  resultats.X = X;
  Serial.printf("\n moteur X step %f", X);
  Serial.printf("\n moteur X step %d", resultats.X);
  resultats.Y = ((Y / reduction[1]) / 360 + 270) * (-1);
  Serial.printf("\n moteur Y step %d", resultats.Y);
  resultats.Z = (Z / reduction[2]) / 360;
  Serial.printf("\n moteur Z step %d", resultats.Z);
  resultats.B = (B / reduction[3]) / 360;
  Serial.printf("\n moteur B step %d", resultats.B);
  //double B = acos(((ZB * ZB) + (YB * YB) - (YZ * YZ)) / (2.00 * YB * ZB)) * 180.0 / M_PI;
  return resultats;
}
*/

void processGCodeLine(String line) {
  // Extraire l'identifiant du moteur et la nouvelle position
  Serial.println();
  char moteurId = line[0];
  if (moteurId == 'G') {
    int values[6];  // Array to store the parsed integers
    int index = 0;  // Index to keep track of the position in the array

    // Split the line into tokens based on space character
    int spaceIndex = line.indexOf(' ');                  // Find the first space (after "G28")
    String restOfLine = line.substring(spaceIndex + 1);  // Ignore "G28"

    // Keep extracting numbers from the string
    while (restOfLine.length() > 0 && index < 6) {
      spaceIndex = restOfLine.indexOf(' ');    // Find the next space
      if (spaceIndex == -1) {                  // No more spaces, this is the last number
        values[index++] = restOfLine.toInt();  // Convert to integer and store
        break;                                 // Exit the loop
      } else {
        values[index++] = restOfLine.substring(0, spaceIndex).toInt();  // Convert to integer and store
        restOfLine = restOfLine.substring(spaceIndex + 1);              // Move to the next part of the string
      }
    }
    Serial.println();
    Serial.print("Extracted values: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(values[i]);
      Serial.print(" ");
    }
    Serial.println();
    stepperX->moveTo(values[0]);
    stepperY->moveTo(values[1]);
    stepperZ->moveTo(values[2]);
    stepperA->moveTo(values[3]);
    stepperB->moveTo(values[4]);
    stepperC->moveTo(values[5]);
  }
  else {
    int nouvellePosition = line.substring(1).toInt();
    // Vérifier quel moteur doit être ajusté et changer sa position
    switch (moteurId) {
      case 'R':
        stepperX->setCurrentPosition(0);
        stepperY->setCurrentPosition(0);
        stepperZ->setCurrentPosition(0);
        stepperA->setCurrentPosition(0);
        stepperB->setCurrentPosition(0);
        stepperC->setCurrentPosition(0);
      case 'X':
        stepperX->moveTo(nouvellePosition);
        Serial.printf("moteur X deplacer a : %d", nouvellePosition);
        break;
      case 'Y':
        stepperY->moveTo(nouvellePosition);
        Serial.printf("moteur Y deplacer a : %d", nouvellePosition);
        break;
      case 'Z':
        stepperZ->moveTo(nouvellePosition);
        Serial.printf("moteur Z deplacer a : %d", nouvellePosition);
        break;
      case 'A':
        stepperA->moveTo(nouvellePosition);
        Serial.printf("moteur A deplacer a : %d", nouvellePosition);
        break;
      case 'B':
        stepperB->moveTo(nouvellePosition);
        Serial.printf("moteur B deplacer a : %d", nouvellePosition);
        break;
      case 'C':
        stepperC->moveTo(nouvellePosition);
        Serial.printf("moteur C deplacer a : %d", nouvellePosition);
        break;
      case 'H':
        if (nouvellePosition == 0) {
          stepperX->moveTo(0);
          stepperY->moveTo(0);
          stepperZ->moveTo(0);
          stepperA->moveTo(0);
          stepperB->moveTo(0);
          stepperC->moveTo(0);
          Serial.printf("moteur home 0");
        } 
        else if (nouvellePosition == 1) {
          stepperX->moveTo(0);
          stepperY->moveTo(-1000);
          stepperZ->moveTo(-1000);
          stepperA->moveTo(0);
          stepperB->moveTo(0);
          stepperC->moveTo(0);
          Serial.printf("moteur home 1");
        }
        break;
      case 'F':
        if (nouvellePosition == 1) {
          function1();
        } else if (nouvellePosition == 2) {
          function2();
        }
        else if (nouvellePosition == 3) {
          function3();
        }
        break;
      case 'U':
        if (nouvellePosition == 0) {
          stepperX->disableOutputs();
          stepperY->disableOutputs();
          stepperZ->disableOutputs();
          stepperA->disableOutputs();
          stepperB->disableOutputs();
          stepperC->disableOutputs();
        } 
        else {
          stepperX->enableOutputs();
          stepperY->enableOutputs();
          stepperZ->enableOutputs();
          stepperA->enableOutputs();
          stepperB->enableOutputs();
          stepperC->enableOutputs();
        }
        break;
      default:
        Serial.printf("Identifiant de moteur invalide");
        break;
    }
  }
}
void function3(void)
{
  String Gcode[] = { "G28 0 -6988 14032 -231 2445 -1588", "G28 0 -11882 5067 -247 1972 -1583"};
  for (int i=0; i<2; i++)
  {
    for (int j=0; j<2; j++)
    {
      processGCodeLine(Gcode[j]);
      while ((stepperX->isRunning()) || (stepperY->isRunning()) || (stepperZ->isRunning()) || (stepperA->isRunning()) || (stepperB->isRunning()) || (stepperC->isRunning())) {}
    }

  }
}