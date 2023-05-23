#ifndef Event_ID_Generate_H
#define Event_ID_Generate_H

#include <ArduinoJson.h>

// Function to generate an 8-digit random event_id
String generate_event_id()
{
    // Generate a random number between 0 and 16777215
    unsigned int random_number = random(16777216);

    // Convert the random number to an 8-digit hexadecimal string
    String event_id = String(random_number, HEX);
    event_id.toLowerCase();
    event_id.trim();

    // Pad the event_id with leading zeros if necessary
    while (event_id.length() < 8)
    {
        event_id = "0" + event_id;
    }

    return event_id;
}

#endif
