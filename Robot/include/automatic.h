// MIT License

// Copyright (c) 2022 Gauthier Dandele

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __AUTOMATIC_H
#define __AUTOMATIC_H

#include <motor.h>
#include <sonar.h>
#include <type.h>

// Distance minimale avant obstacle
// Incréméntée si le robot recule
uint8_t limitDistance = 10;

// Choix d'évitement d'obstacle:
//
// 0 Tourner à gauche
// 1 Tourner à droite
// 2 Revenir position initiale
// 3 Marche arrière
float avoidanceChoice(float time, uint8_t choice) {
  const uint8_t minDistance = 5;
  const uint8_t maxDistance = 20;
  const uint8_t backDistance = 15;
  const float quartTour = 4.40;

  switch (choice) {
    case 0:
      WriteSpeeds(60, -60);
      while (time < quartTour) {
        const uint16_t distance = sonar.read();

        if (distance > maxDistance && time > (quartTour / 2)) {
          limitDistance = 10;
          break;
        } else if (distance < minDistance) {
          break;
        }

        time += 0.1;
        delay(100);
      }

      return time;

    case 1:
      WriteSpeeds(-60, 60);
      while (time > -quartTour) {
        const uint16_t distance = sonar.read();

        if (distance > maxDistance && time < (quartTour / 2)) {
          limitDistance = 10;
          break;
        } else if (distance < minDistance) {
          break;
        }

        time -= 0.1;
        delay(100);
      }

      return time;

    case 2:
      WriteSpeeds(60, -60);
      while (time < 0) {
        time += 0.1;
        delay(100);
      }

      return time;

    case 3:
      WriteSpeeds(-60, -60);
      while (uint16_t(sonar.read()) < (maxDistance + backDistance)) {
      }
      limitDistance += backDistance;
      break;
  }

  return 0;
}

// Mode de fonctionnement automatique
// avec évitement d'obstacle
void automatic() {
  uint8_t choice = 0;
  float TimeRotation = 0;

  WriteSpeeds(255, 255);

  while (uint16_t(sonar.read()) < limitDistance) {
    if (choice > 3) {
      choice = 0;
    }

    TimeRotation = avoidanceChoice(TimeRotation, choice);

    choice++;
    WriteSpeeds(0, 0);

    delay(1000);
  }
}

#endif
