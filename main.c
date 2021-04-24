#include "pico/stdlib.h"
#include "pico/binary_info.h"

const uint LED_PIN = 25;
const char MIDICH = 1;

char MIDIRunningStatus;
char MIDINote;
char MIDILevel;

void doMidiNoteOn(char note, char vel)
{
    gpio_put(LED_PIN, 1);
    //print("Note On \t", note, "\t", vel);
}

void doMidiNoteOff(char note, char vel)
{
    gpio_put(LED_PIN, 0);
    //print("Note Off\t", note, "\t", vel);
}

void doMidi(char mb)
{
    if ((mb >= 0x80) && (mb <= 0xEF))
    {
        // MIDI Voice Category Message.
        // Action: Start handling Running Status
        MIDIRunningStatus = mb;
        MIDINote = 0;
        MIDILevel = 0;
    }
    else if ((mb >= 0xF0) && (mb <= 0xF7))
    {

        // MIDI System Common Category Message.
        // Action: Reset Running Status.
        MIDIRunningStatus = 0;
    }
    else if ((mb >= 0xF8) && (mb <= 0xFF))
    {
        // System Real-Time Message.
        // Action: Ignore these.
        return;
    }
    else
    {
        // MIDI Data
        if (MIDIRunningStatus == 0)
            // No record of what state we're in, so can go no further
            return;

        if (MIDIRunningStatus == (0x80 | (MIDICH - 1)))
        {
            // Note OFF Received
            if (MIDINote == 0)
            {
                // Store the note number
                MIDINote = mb;
            }
            else
            {
                // Already have the note, so store the level
                MIDILevel = mb;
                doMidiNoteOff(MIDINote, MIDILevel);
                MIDINote = 0;
                MIDILevel = 0;
            }
        }
        else if (MIDIRunningStatus == (0x90 | (MIDICH - 1)))
        {
            // Note ON Received
            if (MIDINote == 0)
            {
                // Store the note number
                MIDINote = mb;
            }
            else
            {
                // Already have the note, so store the level
                MIDILevel = mb;
                if (MIDILevel == 0)
                {
                    doMidiNoteOff(MIDINote, MIDILevel);
                }
                else
                {
                    doMidiNoteOn(MIDINote, MIDILevel);
                    MIDINote = 0;
                    MIDILevel = 0;
                }
            }
        }
        else
        {
            // This is a MIDI command we aren't handling right now
            return;
        }
    }
}

int main()
{

    bi_decl(bi_program_description("First Blink"));
    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialise UART 0 to MIDI standard
    uart_init(uart0, 31250);
    //uart_set_format(uart0, 8, 1, UART_PARITY_NONE);

    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    while (1)
    {
        doMidi(uart_getc (uart0));
    }
}