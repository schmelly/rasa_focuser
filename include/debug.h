#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define DEBUG 1

#define serialPrintln(...)                         \
    do                                             \
    {                                              \
        if (DEBUG)                                 \
        {                                          \
            char msg[200];                         \
            memset(msg, '\0', sizeof(char) * 200); \
            sprintf(msg, __VA_ARGS__);             \
            Serial.println(msg);                   \
        }                                          \
    } while (0);
#define serialPrint(...)                           \
    do                                             \
    {                                              \
        if (DEBUG)                                 \
        {                                          \
            char msg[200];                         \
            memset(msg, '\0', sizeof(char) * 200); \
            sprintf(msg, __VA_ARGS__);             \
            Serial.print(msg);                     \
        }                                          \
    } while (0);

#endif // DEBUG_H
