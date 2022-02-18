int N1=4,N2=5,N3=6,N4=7;
int buzzer = 8;
const byte TRIGGER_PIN = 2; // Broche TRIGGER
const byte ECHO_PIN = 3;    // Broche ECHO
/* Constantes pour le timeout: le temps à partir duquel le capteur considère qu'il n'y a aucun obstacle à portée (temps de parcours d'environ 25 ms soit 8m aller-retour. Le capteur n'est pas sensible au delà de 4 m. */
const unsigned long MEASURE_TIMEOUT = 50000UL; // 25ms = ~8m a 340m/s
 
/* Vitesse du son dans l'air en mm/microsecondes */
const float SOUND_SPEED = 340.0 / 1000;
int smokeA0 = A5;
// Your threshold value
int sensorThres = 230;
#define STOP  5
#define MARCHE_ARRIERE  2
#define MARCHE_AVANT  1
#define MARCHE_DROITE  3
#define MARCHE_GAUCHE  4

// blue = tx, violet = RX 

byte commande;
void setup() {
 pinMode(smokeA0, INPUT);
 pinMode(TRIGGER_PIN, OUTPUT);
 digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit etre LOW au repos
 pinMode(ECHO_PIN, INPUT);
 pinMode(N1,OUTPUT);
 pinMode(N2,OUTPUT);
 pinMode(N3,OUTPUT);
 pinMode(N4,OUTPUT); 
 pinMode(buzzer,OUTPUT);
 Serial.begin(9600); 
 Serial2.begin(9600);
 digitalWrite( 17, INPUT_PULLUP );
 Serial.println("AT commands mode***"); 
} 
void loop() { 
 detecte_obst();
 detecte_gaz();
 if (Serial2.available()) {
  commande = Serial2.read();
  if (commande == MARCHE_AVANT)
     marcheAv();
  else if (commande == MARCHE_ARRIERE)
    marcheAr();
  else if (commande == MARCHE_DROITE)
    marcheDr();
  else if (commande == MARCHE_GAUCHE)
    marcheGa();
  else if (commande == STOP)
    stopM();
 }
}
void marcheAr(){
  analogWrite(N1,50);
  digitalWrite(N2,LOW);
  digitalWrite(N3,LOW);
  analogWrite(N4,50);
  }
 void marcheAv(){
  analogWrite(N2,50);
  digitalWrite(N1,LOW);
  digitalWrite(N4,LOW);
  analogWrite(N3,50);
  }
 void marcheGa(){
  digitalWrite(N1,LOW);
  digitalWrite(N2,LOW);
  digitalWrite(N4,LOW);
  analogWrite(N3,50);
  }
 void marcheDr(){
  analogWrite(N2,50);
  digitalWrite(N1,LOW);
  digitalWrite(N3,LOW);
  digitalWrite(N4,LOW);
 }
 void stopM(){
  digitalWrite(N1,LOW);
  digitalWrite(N2,LOW);
  digitalWrite(N3,LOW);
  digitalWrite(N4,LOW);
 }
 void detecte_obst(){
    /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10 microsec sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
 
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son echo (s'il existe) */
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
 
  /* 3. Calcule la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
 
  if(distance_mm/1000 < 0.12){ // CHANGER ICI LA DISTANCE DE DETECTION D'OBSTACLES
    Serial2.println("Un obstacle a été détecté");
    stopM(); //
    delay(1000);
    marcheAr();
    delay(1000);
    stopM();
    Serial2.println("Il n y a plus encore d'obstacle");
  }
 }
  
  void detecte_gaz(){
     int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  if (analogSensor > sensorThres)
  {
    digitalWrite(buzzer, HIGH);
    Serial2.print("Une densité de gaz anormale a été détecté de : ");
    Serial2.println(analogSensor);
    Serial2.print("Ppm");
  }
  else
  {
    digitalWrite(buzzer, LOW);
  
  }
  }
 
   
