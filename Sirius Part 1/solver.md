# SIRIUS – PART I CTF Writeup


## Challenge Overview


The binary presents itself as a poetic Star Themed program. It prints verses, accepts user input, echoes part of it back, and—only under certain circumstances—reveals the secret fragment (the flag)

The goal:
 Trigger the hidden code path in order to call print_flag() and leak the flag.

the challenge is all about reading user input, no prior binary exploitation needed

variables used

<img width="206" height="69" alt="image" src="https://github.com/user-attachments/assets/289090d2-948e-44ab-88cf-78070ab57e73" />


the program reads user input

<img width="589" height="41" alt="image" src="https://github.com/user-attachments/assets/3c213424-6136-4805-bc9f-6ddb14dc85ef" />


stores the big buf into buffer

<img width="436" height="40" alt="image" src="https://github.com/user-attachments/assets/2dffee2a-cb97-49cd-b37f-ebb3879f4513" />


the wordplay here is "shouting to the sky"

<img width="553" height="437" alt="image" src="https://github.com/user-attachments/assets/81c1cb56-f66d-44e3-882c-a7d317cecef2" />

the user needs to shout to the sky, as in inputting a long string of "A"s


<img width="234" height="92" alt="image" src="https://github.com/user-attachments/assets/61926df1-e72b-4f44-acce-6a0f009ef191" />

user input:
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"


and you will get the flag
and a poem...

<img width="813" height="749" alt="image" src="https://github.com/user-attachments/assets/6db1158c-0985-49ea-9a66-81cf2eb1b990" />
