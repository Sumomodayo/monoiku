#include <Arduino.h>

int segPins1[] = {
18, 19, 33, 25, 26, 5, 17
};

int segPins2[] = {
0, 4, 27, 14, 12, 2, 15
};

int numbers[10][7] = {

{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1}  // 9

};

void showNumber(int num, int segPins[]) {

for (int i = 0; i < 7; i++) {

digitalWrite(segPins[i], numbers[num][i]);

}

}

void setup() {

for (int i = 0; i < 7; i++) {

pinMode(segPins1[i], OUTPUT);

}

for (int i = 0; i < 7; i++) {

pinMode(segPins2[i], OUTPUT);

}

}

void loop() {

for (int num = 1; num <= 99; num++) {

int tens = num / 10;

int ones = num % 10;

showNumber(tens, segPins2);

showNumber(ones, segPins1);

delay(1000);

}

}