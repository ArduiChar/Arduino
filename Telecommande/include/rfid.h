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

#ifndef __RFID_H
#define __RFID_H

#include <EEPROM.h>
#include <type.h>

// Check Code RFID reçu:
//
// 0 Robot Verouillé
// 1 Robot Déverrouillé
// 2 Badgez nouvelle carte
// 3 Nouvelle carte ajoutée
// 4 Carte refusée
// 5 Aucune carte ajoutée
int CheckCode(byte Code[4]) {
  // Uncomment for the first write to initialize the memory
  // EEPROM.write(0, 0);

  // Number of Cards saved
  const int len = EEPROM.read(0);

  // Save the first Card
  if (len == 0 && digitalRead(DownPince)) {
    for (int n = 0; n < 4; n++) {
      EEPROM.write((n + 1), Code[n]);
    }

    EEPROM.write(0, 1);
    return 3;
  } else if (len == 0) {
    return 5;
  }

  // Check if the Card matches
  for (int i = 0; i < len; i++) {
    byte Code_Read[] = {};

    for (int n = 0; n < 4; n++) {
      Code_Read[n] = EEPROM.read((i * 4) + n + 1);
    }

    if (Code_Read[0] == Code[0] && Code_Read[1] == Code[1] &&
        Code_Read[2] == Code[2] && Code_Read[3] == Code[3]) {
      if (digitalRead(DownPince)) {
        return 2;
      }

      // Si déverrouillé => Verrouiller
      if (data.RFID_State == 1) {
        return 0;
      }

      return 1;
    }
  }

  return 4;
}

// Save new Card
int NewCard(byte Code[4]) {
  const int len = EEPROM.read(0);

  for (int n = 0; n < 4; n++) {
    EEPROM.write((n + (len * 4) + 1), Code[n]);
  }

  EEPROM.write(0, (len + 1));
  return 3;
}

int RFID(int State) {
  switch (State) {
    case 0:
    case 1:
      State = CheckCode(data.Code);
      break;
    case 2:
      State = NewCard(data.Code);
      break;
  }

  return State;
}

#endif
