// ECE 198 Light Detection Code 
//Modified from Bernie Roehl's starter code on GitLab



 #include <stdbool.h> // booleans, i.e. true and false
 #include <stdio.h>   // sprintf() function
 #include <stdlib.h>  // srand() and random() functions
 #include <string.h>
 #include "ece198.h"


 //functions used for outputting hints 
void morse_output(int * seq){
     //seq refers to an English letter in the form of a Morse Code quadruplet. For example:
     //seq for English letter 'A' would be {1, 3, 0, 0} (1=dot, 3=dash, 0=nothing)
     //Each English letter is guaranteed to have 4 Morse code dots and dashes (including 0s)
     for(int i=0; i<4; i+=1){
     switch(seq[i]){
         case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
         break;
         case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
         HAL_Delay(250);
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
         HAL_Delay(1000);
         break;
         case 3: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
         HAL_Delay(1000);
         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
         HAL_Delay(1000);
         break;
     }
     }
 }

//this function screens each letter of the English alphabet from an array in order to display 
 //a given word in Morse Code
 void spell_out_word(char* word, int word_size, char* corresponding_letters, int seq[26][4]){
 //j keeps track of the current letter in the word that we have to output
 int j=0;
//i keeps track of how many letters of the English alphabet have been screened so far
 int i=0;
     while(i<26){
         if(corresponding_letters[i] == word[j]){
             //seq[i] is a quadruplet representing the jth letter of word
                         morse_output(seq[i]); 
                         HAL_Delay(2000);
                         //increment j by 1 to display next letter
                         j+=1;
                         //set i to 0 so that all letters are screened again (as letters in word)
                         //may not be in alphabetical order)
                         i=0;
                         //exit while loop to prevent unpredictable output after all letters in word have 
                         //been displayed
                        if (j==word_size){
                                i=26;
                            }
                    }
         //keep stepping through the letters of the alphabet until a match is found with word[j]           
        else {i+=1;}
                }
}

//the following function keeps track of whether the hint asked by the user is a duplicate 
bool use_hint(char* word, char* used_hints, char* corresponding_letters, int* morse_combinations,
 int num_of_hints, char hint_letter, int answer_size){
     char hint_letters[2] = {'F', hint_letter}; 
     // the system repeats the hint asked by the user for complete clarity on the user's side
        spell_out_word(hint_letters, 2, corresponding_letters, morse_combinations);
                bool hint_used=false;
                 for(int i=0; i<num_of_hints;i+=1){
                     if(used_hints[i]==hint_letter){
                         hint_used=true;
                         break;
                     }
                 }
                 if(hint_used!=true){
                     //the LED only morses the hint if the hint has not been used, ie., it is not in 
                     //the used_hints list. 
                 used_hints[num_of_hints] = hint_letter;
                 }
                spell_out_word(word, answer_size, corresponding_letters, morse_combinations);
                 return hint_used;
}

int main(void)
    {

     HAL_Init(); // initialize the Hardware Abstraction Layer

     // Peripherals (including GPIOs) are disabled by default to save power, so we
     // use the Reset and Clock Control registers to enable the GPIO peripherals that we're using.

     __HAL_RCC_GPIOA_CLK_ENABLE(); // enable port A (for the on-board LED, for example)
     // __HAL_RCC_GPIOB_CLK_ENABLE(); // enable port B (for the rotary encoder inputs, for example)
     // __HAL_RCC_GPIOC_CLK_ENABLE(); // enable port C (for the on-board blue pushbutton, for example)
     __HAL_RCC_GPIOD_CLK_ENABLE();
     // initialize the pins to be input, output, alternate function, etc...

    InitializePin(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, 0);  // on-board LED/yellow LED
     //the on-board LED is used solely for indication and not actually required.
    InitializePin(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLDOWN, 0); 
     //The hardware circuit passes a logic 1 input to GPIO_PIN_0 if light is incident on the photoresistor
     //and logic 0 if there's no light. 
    InitializePin(GPIOA, GPIO_PIN_6, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0); //controls red LED
    InitializePin(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0); //controls green LED
    InitializePin(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0); // controls blue LED
     // set up for serial communication to the host computer
     // (anything we write to the serial port will appear in the terminal (i.e. serial monitor) in VSCode)

     SerialSetup(9600);
     //the array morse_combinations contains the different combinations of dots (1) and dashes (3) 
     //(breaks signified by 0) to express all 26 letters. corresponding_letters maps morse_combinations
     //(based on array index) to the English letter 
    int morse_combinations[26][4]={{1, 3, 0, 0}, {3, 1, 1, 1}, 
    {3, 1, 3, 1}, {3, 1, 1, 0}, {1, 0, 0, 0}, {1, 1, 3, 1}, {3, 3, 1, 0}, {1, 1, 1, 1}, {1, 1, 0, 0}, 
    {1, 3, 3, 3}, {3, 1, 3, 0}, {1, 3, 1, 1}, {3, 3, 0, 0}, {3, 1, 0, 0}, {3, 3, 3,0},
    {1, 3, 3, 1}, {3, 3, 1, 3}, {1, 3, 1, 0}, {1, 1, 1, 0}, {3, 0, 0, 0}, {1, 1, 3, 0}, {1, 1, 1, 3}
    ,{1, 3, 3, 0}, {3, 1, 1, 3}, {3, 1, 3, 3}, {3, 3, 1, 1}};
    char corresponding_letters[26]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
     //Every English letter consists of 4 or less Morse Code dots/dashes. Therefore, to read each
     //letter inputted, we create an array of 4 int variables. arrays is an array of 3 letters 
     //created to store a maximum of 3 letters.
     //the third letter will ONLY be used for entering the ghost's name as all commands are just
     // two letters
     int arrays[3][4];
    
     //debugging string for terminal output
     char buff[1];
     char buff2[1];
     int num_of_morses = 0; //to track the index of the English letter 
     //sub-arrays within "arrays". Used as a global variable to control program flow.
     uint count = 0; //used to keep track of breaks while the user is entering Morse Code so that
     //letters of less than 4 Morse Code characters can be processed. 
     int index = 0; // used to track the index of each English letter in 'arrays'.
     char letters[3]; // This is used to translate the Morse-Encoded letters stored in "arrays" into 
     //English alphabet.
     int num_of_hints=0; //used to keep track of number of hints so that a cap of 4 is enforced, and 
     //so that incorrect Morse Code is not accidentally counted as a hint
     char used_hints[4]; //used to keep track of duplicate hints. Duplicate hints are not 
     //counted as hints and no answer will be provided for duplicate hints.
    bool turn_on_blue = 1; //when num_of_hints=4, this will activate the blue LED and also signal a condition to switch off

     while (1) //infinite loop that runs for the entire time that microcontroller is on
     //the program is designed so that when num_of_morses == 5, this code block is run and the second letter
     //can be inputted by the user. 
    {   if (num_of_morses==5){
         index=1;
         num_of_morses=0;
     }
     //the below block of code controls acceptance of input for the third letter. This happens when 
     //the number of hints is 4 (user has exhausted all hints) and num_of_morses has been set to 6, 
     //signalling that the second letter has been processed.
     if (num_of_morses==6 && num_of_hints==4){
         index=2;
         num_of_morses=0;
     }
         uint32_t now = HAL_GetTick(); //gets current time
         while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){  
             count=0;    //resets count in case user inputs letter within 
             //time given (before being counted as break). NECESSARY line of code.   
         }
         uint32_t now2 = HAL_GetTick(); 
          //if the inner while loop is actually run, there will be a difference in time recorded at 
         //the beginning ("now") and the time now2 recorded after execution of the inner while loop
         uint32_t elapsed = now2-now;
         if(elapsed>200 && elapsed<=750){
             //short flash, "dit" or "dot" in Morse
             SerialPuts(" dit");
             if(num_of_morses<4){
             arrays[index][num_of_morses] = 1;
             num_of_morses+=1;
             }
            
             }
         else if (elapsed>750){
             //long flash, "dah" or "dash" in Morse
             SerialPuts(" dah");
             if(num_of_morses<4){
             arrays[index][num_of_morses] = 3;
             num_of_morses+=1;
             }
         }
         else{
             //the following code doesn't actually count breaks until after the user has begun to enter a 
             //letter. This allows time for a user to look up the Morse Code for a letter before entering it
             if(num_of_morses<4 && num_of_morses>0){
                 count+=1;
                 //the number of "counts" before the pause is counted as a break (1200000) was 
                 //selected through some basic trial-and-error to determine what a suitable duration would 
                 //be. It may be changed later. 
                    if (count>=1200000)   {
                        SerialPuts("bk ");
         arrays[index][num_of_morses] = 0; 
         num_of_morses+=1; //even if nothing is inputted, it will be counted as a break. 
         count=0; //reset count. Not strictly necessary for this specific application
         // as breaks are usually followed by other breaks in Morse Code. 
         }
             }
         }
         if(num_of_morses==4){
            //for debugging purposes. Can be removed.
             int i=0;
             while(i<4){
                 sprintf(buff, "%d ", arrays[index][i]);
                 SerialPuts(buff);
                 i+=1;
             }
             //The following code does Morse Code decoding into English which is used 
             //for outputting hints. It also modifies num_of_morses and thus controls program flow.
             //j is used to screen the 26 elements of morse_combinations 
             int j=0;
             while(j<26){
                 //the following conditional checks to see if one of the nested arrays within morse_
                 //combinations is exactly equal to the Morse Code entered by the user. If it is,
                 // the corresponding English letter is assigned and recorded in "letters" array.
                 if (morse_combinations[j][0] == arrays[index][0] && morse_combinations[j][1] 
                 == arrays[index][1] && morse_combinations[j][2]==arrays[index][2] && 
                 morse_combinations[j][3] == arrays[index][3]){
                     sprintf(buff, "%c ", corresponding_letters[j]);
                     SerialPuts(buff);
                     letters[index] = corresponding_letters[j]; 
                     //the below code breaks the while loop
                     j=26;
                 }
                 else{
                     //if no match, keep stepping through array
                     j+=1;
                     //the below code defines all elements of letters in case the user enters garbage
                     //required because if left undefined, output can be unpredictable
                     if(j==26){
                         letters[index] = corresponding_letters[j];
                     }
                 }
             } 
             if((letters[0]=='F'&& num_of_hints<4) ||((letters[0]=='A' || letters[0]=='B' || 
             letters[0]=='K')&&num_of_hints>=4)){
                 if(index==0){
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
                 HAL_Delay(2000);
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
                 }
                num_of_morses=5; //important. this is set to signal that one letter has been inputted and 
             //the program can move to reading the second letter of a command. Prevents overwriting.
             //The below code block similarly signals that the second letter has been inputted and the 
             //program can move on to the third letter or process the existing two letters.
             }
             else{
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
                 HAL_Delay(2000);
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
                 num_of_morses=0;
                 index=0;
             }
             
             if(index==1){
                 if(num_of_hints>=4 && 
                 !(letters[1]=='E' || letters[1]=='N'
                  || letters[1]=='M' || letters[1] =='A' ||
                   letters[1]=='L' || letters[1]=='D' || letters[1]=='V')){
                     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
                     HAL_Delay(2000);
                     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
                     num_of_morses=5;
                 }
                 else{
                     if(num_of_hints>=4 ||(num_of_hints<4 && 
                 (letters[1]=='C' || letters[1]=='F'
                  || letters[1]=='P' || letters[1] =='H' ||
                   letters[1]=='L' || letters[1]=='A' || letters[1]=='T'))){
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
                 HAL_Delay(2000);
                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
                     }
                 num_of_morses=6;
                 }
             }
         }
       
         //this code block handles the output. During output, input will still be detected 
         //but not be acted upon. num_of_morses==6 is used to detect that two letters have been entered, 
         //and num_of_hints is used to keep track of the number of hints that have been given. 
        if(num_of_morses==6 && num_of_hints < 4){
            bool hint_used = true; // indicates if the user has already asked this hint
             if(letters[0]=='F'){
                 switch(letters[1]){
                   case 'C':{
                   //LED morses "P-I-N-K"
                 char word[4]= {'P', 'I', 'N', 'K'};
                 hint_used = use_hint(word, used_hints, corresponding_letters, 
                 morse_combinations, num_of_hints, 'C', 4);
                    break;
                   }
                   case  'T':{
                   //LED morses "S-C-A-R-Y"
                   char word2[5] = {'S', 'C', 'A', 'R', 'Y'};
                    hint_used = use_hint(word2, used_hints, corresponding_letters, 
                     morse_combinations, num_of_hints, 'T', 5);
                    break;
                   }
                   case 'P': {
                   //LED morses "D-A-D"
                    char word3[3] = {'D', 'A', 'D'};
                   hint_used = use_hint(word3, used_hints, corresponding_letters, 
                    morse_combinations, num_of_hints, 'P', 3);
                   break;
                   }
                   case 'L': {
                   //LED morses "A"
                    char word4[1] = {'A'};
                   hint_used = use_hint(word4, used_hints, corresponding_letters, 
                    morse_combinations, num_of_hints, 'L', 1);
                    break;
                   }
                   case 'F':{
                   //LED morses "F-R-I-E-S"
                    char word5[5] = {'F', 'R', 'I', 'E', 'S'};
                   hint_used = use_hint(word5, used_hints, corresponding_letters, 
                    morse_combinations, num_of_hints, 'F', 5);
                    break;
                   }
                   case 'H':{
                   //LED morses "S-E-W"
                    char word6[3] = {'S', 'E', 'W'};
                   hint_used = use_hint(word6, used_hints, corresponding_letters, 
                    morse_combinations, num_of_hints, 'H', 3);
                    break;
                   }
                   case 'A':{
                   //LED morses "C-A-T"
                   char word7[3] = {'C', 'A', 'T'};
                  hint_used = use_hint(word7, used_hints, corresponding_letters, 
                    morse_combinations, num_of_hints, 'A', 3);
                   break;
                   }
                   default:
                   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
                   HAL_Delay(2000);
                   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
                   num_of_morses = 5;
                   index=1;  
                 }
             }
             if(!hint_used){
              num_of_hints+=1;  
             }
             if(num_of_morses==6){
              num_of_morses=0; // allows entry of first letter for next hint
              index=0; //backtracks to first letter entry from index=1.
             }
        }
        if(num_of_hints>=4 && turn_on_blue){
            turn_on_blue = 0; 
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 1);
        }
         //the below conditional marks the situation in which the the third letter has been stored from
         //the user. If the 3 letters correspond to the name of the correct ghost, the door is 
        //opened. 
         if(num_of_morses==5){
         if(index==2 && (letters[2]=='O' || letters[2]=='N' || letters[2]=='Y' || letters[2]=='T'
         || letters[2]=='A')){
             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
             HAL_Delay(2000);
             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
             if(letters[0]=='A' && letters[1]=='M' && letters[2]=='Y'){
                   InitializePin(GPIOA, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN, 0); 
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
                    HAL_Delay(1500);
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 40);
                    num_of_morses=8;
             }
             else{
                 num_of_morses = 8; //locks program so that it can't accept more input.
                 sprintf(buff2, "%c ", index);               
                 SerialPuts(buff2);
                 SerialPuts("Hello");
                 char word[5]={'W', 'R', 'O', 'N', 'G'};
                 spell_out_word(word, 5, corresponding_letters, morse_combinations);
             }
              HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
         }
         else if (index==2){
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
            HAL_Delay(2000);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
            num_of_morses=6;  
         }
        
        }
    }


    return 0;
 }



// This function is called by the HAL once every millisecond
 void SysTick_Handler(void)
{
     HAL_IncTick(); // tell HAL that a new tick has happened
//     // we can do other things in here too if we need to, but be careful
 }
