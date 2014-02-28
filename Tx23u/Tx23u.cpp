// Bibliothèque concernant la girouette/anémomètre Tx23 de chez Lacrosse Technologie
// ---------------------------------------------------------------------------------
#include "Tx23u.h"

#define DEFINE_TIMEOUT        500000 // 0,5 secondes (en milisecondes)

// --------------------------------------------------------------------------------
// Constructeur par défaut
// --------------------------------------------------------------------------------
Tx23u::Tx23u()
{
    m_ui_pinIn          = 10 ; // Entrée digitale par défaut
    m_fWindSpeed        = 0 ;
    m_fWindDir          = 0 ;
    m_bReadAskWindDir   = false ; 
    m_bReadAskWindSpeed = false ; 
}

// --------------------------------------------------------------------------------
// Constructeur avec définition de la pin digitale
// --------------------------------------------------------------------------------
Tx23u::Tx23u( uint8_t pinIn )
{
    m_ui_pinIn = pinIn ; // Entrée digitale par défaut
    m_fWindSpeed        = 0 ;
    m_fWindDir          = 0 ;
    m_bReadAskWindDir   = false ; 
    m_bReadAskWindSpeed = false ; 
}

// --------------------------------------------------------------------------------
// Fonction de lecture de la direction du vent (en degrés)
// --------------------------------------------------------------------------------
float Tx23u::GetWindDir( void ) 
{
    if ( !m_bReadAskWindDir ) {
        CollectDataFromTx23();
    }

    m_bReadAskWindDir = false ;

    return(m_fWindDir) ; 
}

// --------------------------------------------------------------------------------
// Fonction de lecture de la vitesse du vent (en m/s)
// --------------------------------------------------------------------------------
float Tx23u::GetWindSpeed( void ) 
{
    if ( !m_bReadAskWindSpeed ) {
        CollectDataFromTx23();
    }

    m_bReadAskWindSpeed = false ;
    return(m_fWindSpeed) ; 
}

// --------------------------------------------------------------------------------
// Fonction d'attente en microsecondes entre deux lectures de bit
// --------------------------------------------------------------------------------
void Tx23u::Tx23Delay()
{
    unsigned long start = micros();
    while ( micros() - start <= DELAY_BIT );
}

// --------------------------------------------------------------------------------
// Fonction de lecture des données sur la Tx23
// --------------------------------------------------------------------------------
void Tx23u::CollectDataFromTx23(void)
{
    char data[50];

    pinMode(m_ui_pinIn, OUTPUT);

    digitalWrite(m_ui_pinIn, HIGH);
    delay(100);

    digitalWrite(m_ui_pinIn, LOW );
    delay(500);

    pinMode(m_ui_pinIn, INPUT);

    #ifdef DEFINE_TIMEOUT                
        unsigned long uTimeEnd = micros() + DEFINE_TIMEOUT ;
        while( digitalRead(m_ui_pinIn) == LOW ){
            if ( micros() > uTimeEnd ) break ;
        };
        uTimeEnd = micros() + DEFINE_TIMEOUT ;
        while( digitalRead(m_ui_pinIn) == HIGH ){
            if ( micros() > uTimeEnd ) break ;
        };
        uTimeEnd = micros() + DEFINE_TIMEOUT ;
        while( digitalRead(m_ui_pinIn) == LOW ){
            if ( micros() > uTimeEnd ) break ;
        };
    #else
        while( digitalRead(m_ui_pinIn) == LOW ){}  ;
        while( digitalRead(m_ui_pinIn) == HIGH ){} ;
        while( digitalRead(m_ui_pinIn) == LOW ){} ;
    #endif

    for (int i = 0 ; i < 42 ; i++ ) {
        data[i] = (digitalRead(m_ui_pinIn) == LOW) ? 41 : 42 ;
        Tx23Delay();
    }

    // ShowAllBits( data );

    DecodeWindir( data );
    m_bReadAskWindDir   = true ; 
    
    DecodeWinSpeed( data );
    m_bReadAskWindSpeed = true ; 
}

// --------------------------------------------------------------------------------
// Fonction d'extraction de la direction du vent
// --------------------------------------------------------------------------------
void Tx23u::DecodeWindir( char *Datas )
{
    unsigned long uWindDir  = 0 ;
    for ( int iCounter = 5 ; iCounter < 9 ; iCounter++) {
        int iBit = ((Datas[iCounter] == 41) ? 0 : 1);
        if ( iBit == 1 ) {
             uWindDir |= (1 << (iCounter-5));
        }
    }

    if (uWindDir == 0) uWindDir = 16 ;

	m_fWindDir = (float)uWindDir * COEF_DIR_TO_DEG ; // en degrés
}

// --------------------------------------------------------------------------------
// Fonction d'extraction de la vitesse du vent
// --------------------------------------------------------------------------------
void Tx23u::DecodeWinSpeed( char *Datas )
{
    unsigned long uWindSpeed = 0 ;
    for ( int iCounter = 9 ; iCounter < 18 ; iCounter++) {
        int iBit = ((Datas[iCounter] == 41) ? 0 : 1);
        if ( iBit == 1 ) {
             uWindSpeed |= (1 << (iCounter-9));
        }
    }

    m_fWindSpeed = (float)uWindSpeed/10.0f ;
}

// ---------------------------------------------------------------------------------------------
// Envoi vers la console l'affichage des bits
// ---------------------------------------------------------------------------------------------
void Tx23u::ShowAllBits( char *data ) 
{
  Serial.println("");

  // HEAD
  for (int i =0 ; i< 5 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }

  Serial.print(" ");
  
  // WINDDIR
  for (int i =5 ; i< 9 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }
  
  Serial.print(" ");
  
  // WIND SPEED
  for (int i =9 ; i < 18 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }
  
  Serial.print(" ");
  
  // WIND SPEED+3
  for (int i =18 ; i< 21 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }  
  
  Serial.print(" ");
  
  // Check sum
  for (int i =21 ; i< 25 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }
  
// 11011 1110 000000000 000 1110 0001 111111111 111
// 01234 5678 901234567 890 1234 5678 901234567 890
//            10         20           30         40
//        Dir   Speed   speed+3 CC   -Dir 
// Bits  0 à 4  : En-tête
//  '    5 à 8  : Direction du vent
//  '    9 à 17 : Vitesse du vent
//  '   18 à 20 : Vitesse du vent + 3
//  '   21 à 24 : 
//  '   25 à 28 : Complément direction du vent
//  '   29 à 37 : Complément vitesse du vent
//  '   38 à 40 : Complément vitesse du vent + 3

  Serial.print(" ");
  
  // WIND DIR  NEGATEIVE
  for (int i =25 ; i< 29 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }
  
    Serial.print(" ");
  
  // WIND SPEED NEGATIVE
  for (int i =29 ; i< 38 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }
   
   Serial.print(" ");
    
    //WIND SPEED NEGATIVE+3
  for (int i = 38 ; i< 41 ; i++){
    Serial.print( ((data[i] == 41)? 0 : 1) );
  }

   Serial.print(" --> ");
   Serial.flush();
}

