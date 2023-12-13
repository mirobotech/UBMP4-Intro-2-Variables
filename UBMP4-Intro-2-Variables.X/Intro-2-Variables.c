/*==============================================================================
 Project: Intro-2-Variables             Activity: mirobo.tech/intro-2
 Date:    December 12, 2023
 
 This introductory programming activity for the mirobo.tech CHRP4 and UBMP4
 demonstrates the use of byte (char) constants and variables to count button
 presses, and conditions to trigger an action when a limit is reached.
  
 Additional program analysis and programming activities demonstrate the use of
 bit (Boolean, or bool) variables to store state for preventing multiple counts
 of the same button being pressed during successive program loops. Additional
 activities include the creation of a two-player rapid-clicker game, simulation
 of a real-world toggle button and multi-function button, and writing code to
 measure and deal with switch contact bounce.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP4.h"           // Include UBMP4 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Program constant definitions
const unsigned char maxCount = 50;

// Program variable definitions
unsigned char SW2Count = 0;
bool SW2Pressed = false;

int main(void)
{
    // The configuration functions run once during program start-up.
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure I/O for on-board UBMP4 devices
    
    // The contents of the while loop repeat continuously.
    while(1)
	{
        // Count SW2 button presses
        if(SW2 == 0)
        {
            LED2 = 1;
            SW2Count = SW2Count + 1;
        }
        else
        {
            LED2 = 0;
        }
        
        // Light LED D3 if the maximum count has been reached
        if(SW2Count >= maxCount)
        {
            LED3 = 1;
        }
        
        // Turn off LED D3 if SW3 is pressed
        if(SW3 == 0)
        {
            LED3 = 0;
            SW2Count = 0;
        }
        
        // Add a short delay to the main while loop.
        __delay_ms(10);
        
        // Reset the microcontroller and start the bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }
    }
}

/* Learn More -- Program Analysis Activities
 * 
 * 1. The 'SW2Count' variable is created as an 8-bit memory location within RAM
 *    by the program declaration: 'unsigned char SW2Count = 0;'
 * 
 *    What is the the maximum value an 8-bit variable can store? What are some
 *    of the benefits and drawbacks of using 8-bit variables in an 8-bit
 *    microcontroller?
 * 
 * 2. The constant 'maxCount' is defined using a declaration similar to that
 *    used for the SW2Count variable, but with the addition of 'const' prefix in
 *    the declaration statement, like this: 'const unsigned char maxCount = 50;'
 * 
 *    The maxCount constant is used to check if the maximum count has been
 *    reached by this statement in the main while loop: if(SW2Count >= maxCount)
 * 
 *    It seems like a lot of work to define a constant. The program could have
 *    simply been written to use this statement instead: if(SW2Count >= 50)
 * 
 *    Can you think of any advantages of declaring a constant first, the way
 *    this program does by using a 'const' declaration statement above the main
 *    code, rather than just embedding the value of the constant into the code
 *    (as shown in the statement above)?
 * 
 * 3. This program should light LED D2 every time SW2 is pressed, and light
 *    LED D3 once the count reaches 50. Try it, and count how many times you
 *    have to press the button until LED D3 turns on. (SW3 resets the count so
 *    you can perform multiple attempts.)
 * 
 *    Did your count reach 50? Can you describe what the program is doing?
 *    (Hint: try pressing and quickly releasing the button, and compare that
 *    with pressing and slowly releasing the button.)
 * 
 * 4. Let's add some debugging code to the second condition block to help figure
 *    out what the program is doing. Modify the second 'if' structure in the
 *    program to add the 'else' structure shown below:

        if(SW2Count >= maxCount)
        {
            LED3 = 1;
        }
        else
        {
            LED3 = 0;
        }

 *    Now, press and hold pushbutton SW2 for at least 10 seconds while watching
 *    LED D3. When the value of SW2 count becomes higher than maxCount, LED D3
 *    should stay lit continuously. Does it do that? Explain why or why not.
 * 
 *    (Hint: If LED D3 turns off while SW2 is being held, even though the
 *    program is set to keep incrementing the SW2Count variable while SW2 is
 *    pressed, you can infer that the value of SW2Count must somehow decrease
 *    below maxCount. Since there are no program statements that decrease the
 *    value of SW2Count, there must be another reason for its value to drop.
 *    Revisit question 1 -- would its limit have an effect?)
 * 
 * 5. We can set a limit on the SW2Count variable by encapsulating its increment
 *    statement inside an if condition statement. In your program, replace the
 *    single line 'SW2Count = SW2Count + 1;' in your program with this new block
 *    of code:
 
            if(SW2Count < 255)
            {
                SW2Count += 1;
            }

 *    This code demonstrates the use of the assignment operator '+=' to shorten
 *    the statement 'SW2Count = SW2Count + 1;' The same operation is performed,
 *    just in a more compact form. After adding this code, what is the maximum
 *    value that the SW2Count variable will reach? How does this affect the
 *    operation of LED D3 when SW2 is held?
 *
 * 6. The fundamental challenge with many microcontroller programs is that the
 *    statements in the while(1) structure of the main() function are run in a
 *    loop. All of the statements in the while(1) loop are repeated, including
 *    the 'if' statement checking the state of pushbutton SW2. Every loop, when
 *    SW2 is checked, another count will be added to SW2Count if SW2 is pressed.
 *    So, the SW2Count variable is not counting new presses of SW2, but instead
 *    is counting the number of while loops for which it remains pressed. And,
 *    since microcontrollers are very fast, the count adds up very quickly!
 * 
 *    Instead of responding to the state of SW2, we want the program to respond
 *    to a *change* of SW2 state, from the not-pressed state to the pressed
 *    state. Doing this requires the use of another variable to store the prior
 *    state of SW2, so that its current state can be evaluated as being either
 *    the same, or different, from its state during the previous iteration of
 *    the main while(1) loop. Replace the initial SW2 'if-else' conditional
 *    structure with the following two if condition structures:

        // Count new SW2 button presses
        if(SW2 == 0 && SW2Pressed == false)
        {
            LED2 = 1;
            SW2Pressed = true;
            if(SW2Count < 255)
            {
                SW2Count = SW2Count + 1;
            }
        }

        // Clear pressed state if SW2 is released
        if(SW2 == 1)
        {
            LED2 = 0;
            SW2Pressed = false;
        }
        
 *    These two new if conditions make use of a Boolean SW2Pressed variable to
 *    store the state of SW2. Boolean variables can store either 0/false, or
 *    1/true. When the SW2Pressed variable was defined near the top of the
 *    program, it was made false with this statment: bool SW2Pressed = false;
 * 
 *    The first 'if' condition now uses the SW2Pressed state as one of the 
 *    conditions when SW2 is pressed. If SW2 is pressed *and* the SW2Pressed
 *    variable is false, the 'if' condition is true representing a new press
 *    of SW2. After turning on LED2, the SW2Pressed variable is made true before
 *    the SW2Count is incremented. 
 * 
 *    After SW2Pressed is made true, the 'if' condition will be false and won't
 *    count the same SW2 press the next time(s) through the loop. Unfortunately,
 *    it will not detect the next legitimate SW2 press either. To do this, the
 *    second 'if' condition is added to reset the SW2Pressed Boolean to false
 *    when SW2 is released, enabling the process to repeat. Try this modified
 *    program code to verify that it works and counts every single SW2 press.
 * 
 *    The conditional statement in the first if condition can also be written:

        if(SW2 == 0 && !SW2Pressed)

 *    The '!SW2Pressed' expression is read as 'not SW2Pressed' and is equivalent
 *    to SW2Pressed being false. Similarly, using the variable name by itself
 *    (eg. SW2Pressed) in a condition would be equivalent to it being true. Try
 *    replacing these in your program and verify that it works.
 * 
 * 7. A pushbutton's logic state can also be defined using a constant definition
 *    to help make the code more readable. Add the following lines to the
 *    'Program constant definitions' section at the top of your program code:
 
#define pressed 0
#define notPressed 1

 *    Now, instead of comparing the state of the button to 0 or 1, the button
 *    input can be compared to the named definitions we just provided, which
 *    helps to make the program more readable. (Though this comes with the
 *    potential problem that not all switches might activate with a 0 level.)
 * 
 *    Try it in your program, and then modify the SW3 reset button code in your
 *    program to work with the same pressed and notPressed definitions.
 
        // Count new SW2 button presses
        if(SW2 == pressed && SW2Pressed == false)
        {
            LED2 = 1;
            SW2Pressed = true;
            if(SW2Count < 255)
            {
                SW2Count = SW2Count + 1;
            }
        }

        // Clear pressed state if released
        if(SW2 == notPressed)
        {
            LED2 = 0;
            SW2Pressed = false;
        }
        
 * 
 * Programming Activities
 * 
 * 1. Can you make a two-player rapid-clicker style game using this program as 
 *    a starting point? Start by using SW5 for the second player's pushbutton.
 *    Duplicate the SW2Count and SW2Pressed variables to create SW5Count and
 *    SW5Pressed variables. Then, duplicate the if condition structures and
 *    modify them to use the variables representing the second player.
 *
 *    LED D3 can still light if player 1 is the first to reach maxCount and is
 *    the winner. Use LED D4 to indicate when player 2 wins. Use a logical
 *    condition statement to reset the game by turning off the winner's LED and
 *    clearing both players' count variables if either SW3 or SW4 is pressed.
 * 
 * 2. Use your knowledge of Boolean variables and logical conditions to simulate
 *    a toggle button. Each new press of the toggle button must 'toggle' an LED
 *    to its opposite state. (Toggle buttons are commonly used as push-on, 
 *    push-off power buttons in digital devices.) Pressing and holding the
 *    toggle button should only cause the LED to toggle once, and not keep
 *    toggling continuously until the button is released.
 * 
 * 3. A multi-function button can be used to enable one action when pressed, and
 *    a second or alternate action when held. A variable that counts loop cycles
 *    can be used to determine how long a button is held (exactly as the first
 *    program did unitentionally because of the loop structure). Make a
 *    multifunction button that lights one LED as soon as a button is pressed,
 *    and lights a second LED after the button is held for more that one second.
 * 
 * 4. Do your pushbuttons bounce? Switch bounce is the term that describes
 *    switch contacts repeatedly closing and opening before settling in their
 *    final (usually closed) state. Switch bounce in a room's light switch is
 *    not a big concern, because we wouldn't notice the lights rapidly flashing
 *    before staying on. But, switch bounce is an issue in a microcontroller
 *    toggle button program because the speed of a microcontroller lets it see
 *    each bounce as a new, separate event, potentially making it perform
 *    multiple actions in response to a single button press.
 * 
 *    Create a program that uses a variable to count the number of times a
 *    pushbutton is pressed and to then display the count on the LEDs. Use a
 *    second pushbutton to reset the count and to turn off the LEDs so that the
 *    test can be repeated. To determine if your switches bounce, try pressing
 *    them at various rates of speed and using different amounts of force.
 * 
 * 5. Did your pushbuttons bounce? Can you think of a technique similar to the
 *    multi-function button that could be implemented to allow your program to
 *    ignore switch bounces?
 * 
 *    Typical switch bounces appear as multiple switch activations within an
 *    approximatley 20ms time span. So, if a program can be made to ignore any
 *    new switch activations for 20ms after the first switch closure, it should
 *    totally eliminate the effects of switch bounce on its operation. See if
 *    you can add switch de-bouncing code to your bounce counting program in 4,
 *    above, and then test it to see how effective it is.
 */
