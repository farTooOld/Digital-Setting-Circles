/* • APPLICATION: Code for DSC.  This code uses the Encoder library (see the link in
                  the info panel when you add the library to Arduino IDE). 
                 
                  Date: 9/4/17 by; B. Troyan, GalaxyPoint.com

  COMMENTS:       This code contains enough to help you fully complete your project - 
                  I deleted some code that you will need to complete but, careful
                  inspection and reading the comments will make it easy. In some
                  instances I left code, in other places I deleted and placed comments.

                  The code will match up with the Circuit and PCB posted at 
                  http://fritzing.org/projects/telescope-tracker
                  so, all you need to do is complete what's missing...
                  In the end, you will have:
                  Two Encoders for the scope position (Azi and Alt)
                  A button to set Azi & Alt to 0,0
                  A button to set Azi to 0 and Alt to 45 deg. ★ change it to your
                  latitude. 
                  One Encoder for manually setting Azi & Alt to a reference star
                  (Example: Press setAlt, turn the manual encoder to desired value and
                  press it's shaft - the spec'd encoder contains a button switch that
                  will set the value to what you dialed in). You can use the spec'd
                  encoder or buy one without detents...

                  You can use the serial monitor but, be aware that this may slow the
                  responsivness of encoder rotation/updates.  Comment them for your
                  final design with an LCD...
           
*/
// **************************************************************************************
// *                           ***** DECLARATIONS *****                                 *
// **************************************************************************************
#include <Encoder.h>
#include <LiquidCrystal.h>
#include<stdlib.h>                          // custom C formating of strings
#include <avr/io.h>
#include <util/delay.h>                     // reqd by C to use delays

LiquidCrystal lcd(10,9,7,6,5,4);            // changed pins for clean wire layout

Encoder enc_A(3, 19);                       // Pins D3, A5: D3 intrupt, A5 no intrpt
                                            // Analog A5 + 14 = D19 on Atmel 328p & UNO
Encoder enc_MANUAL(16,17);                  // Pins A2, A3

volatile float Azi = 0.0, Alt = 0.0;        // Azi and Alt position
char stringBuff1[8];                        // Alt output buffer for LCD

const char degree = char (223);             // degree symbol for extended ascii
const short setZERO = 11;                   // D11 Alt & Azi 0,0 reset btn
const short polarisBtn = 12;                // D12
const short setAZI = 13;                    // D13 = setAZI
const short setALT = 14;                    // A0 = D14 = setALT
const short setManBtn = 15;                 // A1 = D15 = SET

                          /* ★★★ Coefficients for different encoders and gearing ★★★
                          >> 360/PPRenc/GR/Q << Use '4' for 'Q'uadrature, '1' = None  
                            ∴ 360deg / PPR pulses / GR / Q = "X deg/pulse "  */
float degSignWise = 0.0375;                 // √ = 360/600/4/4 (GR 4:1)

float EncA_Coef = degSignWise;              // ★★★  Set for desired encoder coef
float EncB_Coef = degSignWise;              // ★★★  Set for desired encoder coef

                                       // ★ Also change 'OVER-ROTATION OVERFLOW' below
float oldPosition = 0;

// **************************************************************************************
// *                               ***** MAIN *****                                     *
// **************************************************************************************
int main(void) {
  lcd.begin(16, 2);                        // LCD's number of columns and rows
  startup();                               // see sub's below... welcome msg
  lcdStuff();                              // see sub's below... sets up lcd...

  pinMode(16, INPUT_PULLUP);               // A2 enc_MANUAL encoder
  pinMode(17, INPUT_PULLUP);               // A3 enc_MANUAL encoder

  pinMode(3, INPUT_PULLUP);                // Encoder 1
  pinMode(19, INPUT_PULLUP);               // Encoder 1
  /* ★ Your code for Encoder 2 */
  
  pinMode(polarisBtn, INPUT_PULLUP);       // D4  Polaris btn
  pinMode(setZERO, INPUT_PULLUP);          // D5  Alt & Azi reset btn
  pinMode(setALT, INPUT_PULLUP);           // A0 + 14 = D14, set Alt coord btn
  pinMode(setAZI, INPUT_PULLUP);           // D13 set Azi coord btn
  pinMode(setManBtn, INPUT_PULLUP);        // A1 + 14 = D15, set btn

  digitalWrite(setZERO, HIGH);     // set high
  digitalWrite(polarisBtn, HIGH);          // set high
  digitalWrite(setALT, HIGH);              // set high
  digitalWrite(setAZI, HIGH);              // set high
  digitalWrite(setManBtn, HIGH);           // set high

  long A_position  = -999;
  long B_position  = -999;

  Serial.begin(9600);

  // ************************************************************************************
  // *                          ***** While LOOP *****                                  *
  // ************************************************************************************
  while (1) {
    long new_A, new_B, new_C;                  // keep here or it hiccups!
    new_A = enc_A.read();                      // A is ALTitude
  __asm__("nop\n");                            // sync delay
                                               // ★★ your code for the AZImuth
  __asm__("nop\n");                            // sync delay
    new_C = enc_MANUAL.read();                 // C is manual encoder
  __asm__("nop\n");                            // sync delay

 if (new_A != A_position || new_B != B_position) {    // this is for two encoders

 // ---------------------- >>>★★★ OVER-ROTATION OVERFLOW <<<★★★
/* **** Encoder A **** */
      if (new_A <= -9600 ) {                   // ★ Fixes neg over-flow ★
        new_A = 9600;                          //  new_B, new_A are signed ints
      }                                 // ★★★ There are X counts per Scope rev: ★★★
                                              // X = PPRenc * Q * GR. Be sure to set
                                              // it using your Encoder's PPR per
                                              //  encoder rev.
                                              // √ SignWise: 600 * 4 * 4 = 9600

/*  ★★ Your code for Encoder  B the AZImuth **** */

//-----------------------------------------------

      A_position = new_A;
      B_position = new_B;

      Alt = new_A * EncA_Coef;                  // ★★★ Be sure to set in Declarations
      Azi = new_B * EncB_Coef;                  // ★★★

      // Alt roll over -----------------------
     if (Alt >= 360.0 ) {
       enc_A.write(0);
       Alt = Alt + EncA_Coef;
      }

     if (Alt < 0.0 ) {
        Alt = 360.0 + (new_A * EncA_Coef);    // new_A negative when it gets here
      }// end Alt roll -----

      // Azi roll over ------------------------
        /* ★★ Your code for Azimuth rollover goes here  */

//-----------------------------------------------
  // DISPLAY
     dtostrf(Alt, 7, 3, stringBuff1);                // 7 char wide with 3 decimals
     lcd.setCursor(6, 0);   lcd.print(stringBuff1); lcd.print(degree);  // in one-line

      /* ★★ Also do Your code for displaying theAzimuth */
    
    Serial.println(Alt); 
  //  Serial.print("    "); 
  //  Serial.println(Azi);
    
    }//•end if New Postion at the top -fyi: put cursor in gutter to see fold line start

  // BUTTONS
    if (digitalRead(setZERO) == LOW) {                // • 0,0 btn pressed
      enc_A.write(0);                                 // write 0 to enc_A
                                            // ★Your code - do it for Azi encoder too!
      lcdStuff();                                     // clears lcd leftovers
    }//•end btn if

    if (digitalRead(polarisBtn) == LOW) {             // • polaris btn pressed
      enc_A.write(1210);                              // • write to enc_A: The value that
                                                      // represents your latitude
                                                      // Also, write 0 to Azi encoder
                                                      //• X = Latitude*PPR*Gr*Q / 360
                                                      //• X = 45.4*600*4*4/360 = 1210
    }//•end btn if

// Manually Setting Altitude
    if (digitalRead(setALT) == LOW)   {
       __asm__("nop\n");

      while(1) {                                      // do this until break/exit
        float newPosition = enc_MANUAL.read() * 0.5;  //★ mult by 0.5 for 1/2 deg steps
                                                      // if desired. Or, don't you decide
        __asm__("nop\n");   
       if (newPosition != oldPosition) {
          _delay_ms(2);
          oldPosition = newPosition;
             // Alt roll over ------------------------
             if (newPosition >= 360.0 ) {
               enc_MANUAL.write(0);
               newPosition = 0;
              }//endid
        
             if (newPosition < 0.0 ) {
                newPosition = 360.0 + (newPosition); // new is negative when it gets here
              }// end Alt roll -----

      //   Serial.println(newPosition);
         enc_A.write(newPosition/EncA_Coef);
         Alt = newPosition;
         dtostrf(Alt, 7, 3, stringBuff1);
         lcd.setCursor(6, 0);   lcd.print(stringBuff1); lcd.print(degree); // in one-line
        Serial.println(Alt); 

        }//endIf

        if (digitalRead(setManBtn) == LOW) {
              enc_MANUAL.write(0);                 // reset for new manual counting
              newPosition = 0;                     // reset for new manual counting
           break;                                  // exits out of while loop
        }//endif
      }//end While
     }//endif

// ★★★ Manually Setting Azimuth ★★ ----------------------------------
  /** Your code for the Azimuth (basically, copy the altitude code and tweak names...) */

  }//•end while----------------------------------------

  return (0);                                     // never gets here but used by C
}// ••• end main •••
// **************************************************************************************
// *                              ***** • SUBROUTINES • *****                           *
// **************************************************************************************
void lcdStuff() {
     lcd.setCursor(0, 0);                    // persistant static text display
     lcd.print("Alt = ");
     lcd.setCursor(15, 0);

   Serial.print(Alt); 
   //Serial.print("    "); 
   //Serial.println(Azi);
}//end lcdStuff

void startup() {                             // Basically just a welcome message
     lcd.setCursor(0, 0);
     lcd.print("Starting...");
     _delay_ms(1200);                        // display for 1.2 sec's
     lcd.clear();
}//end startup

