/*
 * Izradio: Nikola Dunaj
 * 02.05.2021.
 */
// pozivanje biblioteka
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// definiranje adrese i veličine LCD zaslona
LiquidCrystal_I2C lcd(0x27,20,4);

//deklaracija i inicijalizacija varijabli
unsigned long start_t;
unsigned long trenutno_t;
unsigned long proteklo_t;
long nasumic;
int nasumic_boja; 
int nasumic_frekvencija;
int n;
int dij = 0;
float kalibracija = 0;

int uzorak;
int varanje = 0;

int LED_c = 2;
int LED_z = 3;
int LED_p = 4;

int TIPK_1 = 5;
int TIPK_c = 6;
int TIPK_z = 7;
int TIPK_p = 8;

int zvucnik = 11;
int piezo = 12;

void setup() {
  
 //postavljanje digitalnih ulaza
 pinMode(TIPK_1, INPUT);
 pinMode(TIPK_c, INPUT);
 pinMode(TIPK_z, INPUT);
 pinMode(TIPK_p, INPUT);
 
 //omogućavanje internih pull-up otpornika
 digitalWrite(TIPK_1,HIGH);
 digitalWrite(TIPK_c,HIGH);
 digitalWrite(TIPK_z,HIGH);
 digitalWrite(TIPK_p,HIGH);

 //postavljanje digitalnih izlaza
 pinMode(LED_c, OUTPUT);
 pinMode(LED_z, OUTPUT);
 pinMode(LED_p, OUTPUT);
 pinMode(zvucnik, OUTPUT);
 pinMode(piezo, OUTPUT);

 //inicijalizacija stanja RGB diode
 digitalWrite(LED_c, HIGH);
 digitalWrite(LED_z, HIGH);
 digitalWrite(LED_p ,HIGH);
 
 //inicijalizacija lcd zaslona
 lcd.init();
 //uključi pozadinsko svjetlo
 lcd.backlight();

 //inicijalizacija serijske komunikacije
 Serial.begin(9600);
 
}

void loop() {

  //izbornik
  izbornik:
  varanje = 0;
  uzorak = 0;
  dij = 0;
  kalibracija = 0;
  
  //ispis izbornika
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Dijagnostika      ->");
  lcd.setCursor(0,1);
  lcd.print("Vrijeme refleksa  ->");
  lcd.setCursor(0,2);
  lcd.print("Refleks na boje   ->");
  lcd.setCursor(0,3);
  lcd.print("Refleks na zvuk   ->");

  //delay zbog petlje
  delay(1100);
  
  izb_poc:
  if(digitalRead(TIPK_1) == LOW){
    
    //signaliziraj pritisnuto tipkalo zvukom
    tone(piezo, 9000, 50);
    goto dijagnostika;
  }
  
  if(digitalRead(TIPK_c) == LOW){
    
    //signaliziraj pritisnuto tipkalo zvukom
    tone(piezo, 9000, 50);
    goto refleks;
  }
  
  if(digitalRead(TIPK_z) == LOW){
    
    //signaliziraj pritisnuto tipkalo zvukom
    tone(piezo, 9000, 50);
    goto refleks_boje;
  }
  
  if(digitalRead(TIPK_p) == LOW){
    
    //signaliziraj pritisnuto tipkalo zvukom
    tone(piezo, 9000, 50);
    goto refleks_zvuk;
  }
  
  //ako ništa nije pritisnuto, vraćaj se na početak i ispituj ponovo
  if (digitalRead(TIPK_1)==HIGH&&digitalRead(TIPK_c)==HIGH&&digitalRead(TIPK_z)==HIGH&&digitalRead(TIPK_p)==HIGH){
    delay(100);
    goto izb_poc;
  }


/////////////////////////////////////////dijagnostika
  dijagnostika:
  
  //ispis na zaslon
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("    DIJAGNOSTIKA");
  lcd.setCursor(0,1);
  lcd.print("Svijetle li     NE->");
  lcd.setCursor(0,2);
  lcd.print("sve boje        DA->");

  
 //Testiranje jesu li ispravne svjetleće diode
 for(n=0; n<4; n++){
   digitalWrite(LED_c, LOW);
   digitalWrite(LED_z, HIGH);
   digitalWrite(LED_p, HIGH);
   delay(100);
   digitalWrite(LED_c, HIGH);
   digitalWrite(LED_z, LOW);
   digitalWrite(LED_p, HIGH);
   delay(100);
   digitalWrite(LED_c, HIGH);
   digitalWrite(LED_z, HIGH);
   digitalWrite(LED_p, LOW);
   delay(100);
   digitalWrite(LED_p, HIGH);
 }

 dij_LED:
    if(digitalRead(TIPK_c) == HIGH && digitalRead(TIPK_z) == HIGH){
      goto dij_LED;
    }
    if(digitalRead(TIPK_z) == LOW || digitalRead(TIPK_c) == LOW){
      
      //signaliziraj pritisnuto tipkalo zvukom
      tone(piezo, 9000, 50);
      
      //ugasi sve LED
      digitalWrite(LED_c, HIGH);
      digitalWrite(LED_z, HIGH);
      digitalWrite(LED_p, HIGH);
      if (digitalRead(TIPK_z) == LOW){
        
        //signaliziraj pritisnuto tipkalo zvukom
        tone(piezo, 9000, 50);
        dij++;
      }
      
      if (digitalRead(TIPK_c) == LOW){
        
        //signaliziraj pritisnuto tipkalo zvukom
        tone(piezo, 9000, 50);
      }
    }
    delay(500);

    //ispis poruke na zaslon
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    DIJAGNOSTIKA");
    lcd.setCursor(0,1);
    lcd.print("Cuje li se      NE->");
    lcd.setCursor(0,2);
    lcd.print("zvuk?           DA->");

    //stvaranje tonova različitih frekvencija
    for(n=1000; n<=3000; n++){
      delay(1);
      tone(zvucnik, n);
    }
    
   dij_zvuk:
      if(digitalRead(TIPK_c) == HIGH && digitalRead(TIPK_z) == HIGH){
        goto dij_zvuk;
      }
      if(digitalRead(TIPK_z) == LOW || digitalRead(TIPK_c) == LOW){
        
        //signaliziraj pritisnuto tipkalo zvukom
        tone(piezo, 9000, 50);
        
         noTone(zvucnik);
         
        if (digitalRead(TIPK_z) == LOW){
          
          //signaliziraj pritisnuto tipkalo zvukom
          tone(piezo, 9000, 50);
          dij++;
        }
        
        if (digitalRead(TIPK_c) == LOW){
          
          //signaliziraj pritisnuto tipkalo zvukom
          tone(piezo, 9000, 50);
        }
      }

    //ispis poruke na zaslon
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    DIJAGNOSTIKA");
    lcd.setCursor(0,1);
    lcd.print("Kalibrirati?    NE->");
    lcd.setCursor(0,2);
    lcd.print("                DA->");

    dij_kalib:
      if(digitalRead(TIPK_c) == HIGH && digitalRead(TIPK_z) == HIGH){
        goto dij_kalib;
      }
      if(digitalRead(TIPK_z) == LOW || digitalRead(TIPK_c) == LOW){
        
        //signaliziraj pritisnuto tipkalo zvukom
        tone(piezo, 9000, 50);
         
        if (digitalRead(TIPK_z) == LOW){
          
          //signaliziraj pritisnuto tipkalo zvukom
          tone(piezo, 9000, 50);
          kalib:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("    KALIBRACIJA     ");       
        
          //uzimaj 10 uzorka
          if (uzorak == 10){
            
            //signaliziraj kraj testa
            tone(piezo, 12000, 50);
            delay(250);
            tone(piezo, 12000, 50);
            delay(250);
            tone(piezo, 12000, 50);
            
            kalibracija = kalibracija / 10.0000;

            //ispis prosječnog kašnjenja
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("    KALIBRACIJA     ");
            lcd.setCursor(0,2);
            lcd.print("prosjecno kasnjenje:");
            lcd.setCursor(0,3);
            lcd.print("      ");
            lcd.print(kalibracija);
            
            //ispis znaka mikro
            lcd.print(" \344S");
            delay(4000);
            goto zavrsetak;
          }
          
          uzorak ++;
          
          //stvaranje nasumičnog vremena 
          nasumic = random(1000, 2000);
          delay(nasumic);
          start_kalib:
          
          //ugasi crvenu svjetleću diodu
          digitalWrite(LED_c, HIGH);
          
          //ako je zabilježen pokušaj varanja
          if(varanje > 0){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print(" ZABILJEZEN POKUSAJ");
            lcd.setCursor(0,2);
            lcd.print("      VARANJA");
            delay(2000);
            
            goto zavrsetak;
          }
        
          if(digitalRead(TIPK_1) == LOW){
            
            varanje ++; //ako je tipkalo pritisnuto prije pojave svjetla
            goto start_kalib;
          }
          
          if(digitalRead(TIPK_1) == HIGH){
            start_t = micros();
            poc_kalib:
            digitalWrite(LED_c, LOW); 
            if(digitalRead(TIPK_1) == HIGH){
              
              //ako dioda svjetli vise od 5 sekundi a tipkalo nije pritisnuto
              if((micros()-start_t)>5000000){
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("   Nije reagirano");
                delay(2000);
                digitalWrite(LED_c, HIGH);
                goto kalib;
              }
              goto poc_kalib;
            }
            if(digitalRead(TIPK_1) == LOW){
              trenutno_t = micros();
              proteklo_t = trenutno_t - start_t;
              
              //ispis vremena u us na 3 decimale
              Serial.println(proteklo_t);
              digitalWrite(LED_c, HIGH);
              kalibracija += proteklo_t;
              goto kalib;
            }
          }
          
        }
        
        if (digitalRead(TIPK_c) == LOW){
          
          //signaliziraj pritisnuto tipkalo zvukom
          tone(piezo, 9000, 50);
        }       
      }
      
      zavrsetak:
      
      //ako rade sve komponente
      if(dij==2){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(" Uredaj je spreman");
        lcd.setCursor(0,2);
        lcd.print("   za koristenje");
        delay(2000);
      }
      
      //ako ne rade sve komponente
      if(dij<2){
        for(n=0; n<3; n++){
            tone(piezo, 12000, 500);
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print(" Uredaj je potrebno");
            lcd.setCursor(0,2);
            lcd.print("    SERVISIRATI");
            delay(2000);
            lcd.clear();
            delay(500);
        }
      }
    
  goto izbornik;
/////////////////////////////////////////test refleksa na svjetlost
  
  refleks:
  varanje = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   TEST REFLEKSA  ->");
  lcd.setCursor(0,2);
  lcd.print("    sivo tipkalo");

  //uzimaj 10 uzorka
  if (uzorak == 10){
    
    //signaliziraj kraj testa
    tone(piezo, 12000, 50);
    delay(250);
    tone(piezo, 12000, 50);
    delay(250);
    tone(piezo, 12000, 50);
    goto izbornik;
  }
  
  uzorak ++;
  
  //stvaranje nasumičnog vremena 
  nasumic = random(1000, 4000);
  delay(nasumic);
  start:
  
  //ugasi crvenu svjetleću diodu
  digitalWrite(LED_c, HIGH);
  
  //ako je zabilježen pokušaj varanja
  if(varanje > 0){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(" ZABILJEZEN POKUSAJ");
    lcd.setCursor(0,2);
    lcd.print("      VARANJA");
    delay(2000);
    Serial.println(); 
    goto refleks;
  }
  
  //ako je tipkalo pritisnuto prije pojave svjetla
  if(digitalRead(TIPK_1) == LOW){
    varanje ++; 
    goto start;
  }
  
  if(digitalRead(TIPK_1) == HIGH){
    start_t = micros();
    poc:
    digitalWrite(LED_c, LOW); 
    if(digitalRead(TIPK_1) == HIGH){
      
      //ako dioda svjetli vise od 5 sekundi a tipkalo nije pritisnuto
      if((micros()-start_t)>5000000){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("   Nije reagirano");
        delay(2000);
        digitalWrite(LED_c, HIGH);
        Serial.println();
        goto refleks;
      }
      goto poc;
    }
    if(digitalRead(TIPK_1) == LOW){
      trenutno_t = micros();
      proteklo_t = trenutno_t - start_t;

      //ispis rezultata
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   TEST REFLEKSA  ->");
      lcd.setCursor(0,3);
      lcd.print(" Vrijeme: ");
      lcd.print(proteklo_t/1000.000);
      lcd.print(" ms");
  
      //ispis vremena u ms na 3 decimale
      Serial.println(proteklo_t/1000.000);
      digitalWrite(LED_c, HIGH);
      delay(1000);
      goto refleks;
    }
  }
/////////////////////////////////////////test refleksa na boje

refleks_boje:
  varanje = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  REFLEKS NA BOJE   ");
  lcd.setCursor(0,1);
  lcd.print("Pritisnite na     ->");
  lcd.setCursor(0,2);
  lcd.print("tipkalo           ->");
  lcd.setCursor(0,3);
  lcd.print("odgovarajuce boje ->");

  //uzimaj 10 uzorka
  if (uzorak == 10){
    
    //signaliziraj kraj testa
    tone(piezo, 12000, 50);
    delay(250);
    tone(piezo, 12000, 50);
    delay(250);
    tone(piezo, 12000, 50);
    
    goto izbornik;
  }
  uzorak ++;
  
  //stvaranje nasumičnog vremena 
  nasumic = random(1000, 4000);
  nasumic_boja = random(1, 4);
  delay(nasumic);
  start_b:
  digitalWrite(LED_c, HIGH);
  digitalWrite(LED_z, HIGH);
  digitalWrite(LED_p, HIGH);
  
  //ako je zabilježen pokušaj varanja
  if(varanje > 0){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(" ZABILJEZEN POKUSAJ");
    lcd.setCursor(0,2);
    lcd.print("      VARANJA");
    delay(2000);
    Serial.println(); 
    goto refleks_boje;
    
  }
  
  if(digitalRead(TIPK_c) == LOW || digitalRead(TIPK_z) == LOW || digitalRead(TIPK_p) == LOW){
    
    //ako je tipkalo pritisnuto prije pojave svjetla
    varanje ++; 
    goto start_b;
  }
  
  //ako je nasumicna boja crvena
  if(nasumic_boja == 1){
    if(digitalRead(TIPK_c) == HIGH){
      start_t = micros();
      poc_bc:
      digitalWrite(LED_c, LOW); 
        if(digitalRead(TIPK_c) == HIGH){
          
          //ukoliko nije reagirano na svjetlo 5 sekundi
          if((micros()-start_t)>5000000){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("   Nije reagirano");
            delay(2000);
            digitalWrite(LED_c, HIGH);
            Serial.println();
            goto refleks_boje;
          }
          goto poc_bc;
        }
        if(digitalRead(TIPK_c) == LOW){
          trenutno_t = micros();
          proteklo_t = trenutno_t - start_t;

          //ispis rezultata
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  REFLEKS NA BOJE   ");
          lcd.setCursor(0,3);
          lcd.print(" Vrijeme: ");
          lcd.print(proteklo_t/1000.000);
          lcd.print(" ms");
      
          //ispis vremena u ms na 3 decimale
          Serial.println(proteklo_t/1000.000);
          digitalWrite(LED_c, HIGH);
          delay(1000);
          goto refleks_boje;
        }
    }
  }

  //ako je nasumicna boja zelena
  if(nasumic_boja == 2){
    if(digitalRead(TIPK_z) == HIGH){
      start_t = micros();
      poc_bz:
      digitalWrite(LED_z, LOW); 
        if(digitalRead(TIPK_z) == HIGH){
          
          //ukoliko nije reagirano na svjetlo 5 sekundi
            if((micros()-start_t)>5000000){
              lcd.clear();
              lcd.setCursor(0,1);
              lcd.print("   Nije reagirano");
              Serial.println();
              digitalWrite(LED_z, HIGH);
              delay(2000);
              
              goto refleks_boje;
            }
            goto poc_bz;
        }
        if(digitalRead(TIPK_z) == LOW){
          trenutno_t = micros();
          proteklo_t = trenutno_t - start_t;
          
          //ispis rezultata
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  REFLEKS NA BOJE   ");
          lcd.setCursor(0,3);
          lcd.print(" Vrijeme: ");
          lcd.print(proteklo_t/1000.000);
          lcd.print(" ms");
          
          //ispis vremena u ms na 3 decimale
          Serial.println(proteklo_t/1000.000);
          digitalWrite(LED_z, HIGH);
          delay(1000);
          goto refleks_boje;
        }
    }
  }

  //ako je nasumicna boja plava
  if(nasumic_boja == 3){
    if(digitalRead(TIPK_p) == HIGH){
      start_t = micros();
      poc_bp:
      digitalWrite(LED_p, LOW); 
        if(digitalRead(TIPK_p) == HIGH){
          //ukoliko nije reagirano na svjetlo 5 sekundi
          if((micros()-start_t)>5000000){
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("   Nije reagirano");
            Serial.println();
            digitalWrite(LED_c, HIGH);
            delay(2000);
            
            goto refleks_boje;
           }
          goto poc_bp;
        }
        if(digitalRead(TIPK_p) == LOW){
          trenutno_t = micros();
          proteklo_t = trenutno_t - start_t;
          
          //ispis rezultata
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  REFLEKS NA BOJE   ");
          lcd.setCursor(0,3);
          lcd.print(" Vrijeme: ");
          lcd.print(proteklo_t/1000.000);
          lcd.print(" ms");
          
          //ispis vremena u ms na 3 decimale
          Serial.println(proteklo_t/1000.000);
          digitalWrite(LED_p, HIGH);
          delay(1000);
          goto refleks_boje;
        }
    }
  }

/////////////////////////////////////////test refleksa na zvuk
 refleks_zvuk:
  varanje = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" REFLEKS NA ZVUK  ->");
  lcd.setCursor(0,3);
  lcd.print("    sivo tipkalo");

  //uzimaj 10 uzorka
  if (uzorak == 10){
    
    //signaliziraj kraj testa
    tone(piezo, 12000, 50);
    delay(250);
    tone(piezo, 12000, 50);
    delay(250);
    tone(piezo, 12000, 50);
    goto izbornik;
  }
  
  uzorak ++;
  
  //stvaranje nasumičnog vremena 
  nasumic = random(1000, 4000);
  delay(nasumic);
  nasumic_frekvencija = random(1000, 18000);
  poc_zv:
  
  //ako je zabilježen pokušaj varanja
  if(varanje > 0){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(" ZABILJEZEN POKUSAJ");
    lcd.setCursor(0,2);
    lcd.print("      VARANJA");
    noTone(zvucnik);
    delay(2000);
    Serial.println(); 
    goto refleks_zvuk;
  }
  
  tone(zvucnik, nasumic_frekvencija);
  if(digitalRead(TIPK_1) == LOW){
    //ako je tipkalo pritisnuto prije pojave svjetla
    varanje ++; 
    goto poc_zv;
  }
  
  if(digitalRead(TIPK_1) == HIGH){
    start_t = micros();
    poc_zv1:
    
    if(digitalRead(TIPK_1) == HIGH){
      
      //ako zvučnik signalizira vise od 5 sekundi, a tipkalo nije pritisnuto
      if((micros()-start_t)>5000000){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("   Nije reagirano");
        Serial.println();
        delay(2000);      
        noTone(zvucnik);
        goto refleks_zvuk;
      }
      goto poc_zv1;
    }
 
    if(digitalRead(TIPK_1) == LOW){
      trenutno_t = micros();
      proteklo_t = trenutno_t - start_t;

      //ispis rezultata
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" REFLEKS NA ZVUK  ->");
      lcd.setCursor(0,3);
      lcd.print(" Vrijeme: ");
      lcd.print(proteklo_t/1000.000);
      lcd.print(" ms");
      
      //ispis vremena u ms na 3 decimale
      Serial.println(proteklo_t/1000.000);

      noTone(zvucnik);
      delay(1000);
      goto refleks_zvuk;
    }
  }
}
