// ===============================================================================================
// Bibliothèque concernant la girouette/anémomètre Tx23h de chez Lacrosse Technologie
// Copyright ph. Schnellbach - 2014
// ===============================================================================================
#ifndef TX23U_h
    
    #define TX23U_h

    #if (ARDUINO >= 100)
	    #include <Arduino.h>
    #else
	    #include <WProgram.h>
    #endif

    #define COEF_DIR_TO_DEG         22.5f
    #define DELAY_BIT               1230L

    class Tx23u {
	    public :
		    Tx23u();
            Tx23u( uint8_t pinIn );     // Initialisation avec une entrée digitale autre que la pin 10
		    float GetWindDir( void ) ;   // Retourne la direction du vent (en °)
		    float GetWindSpeed( void ) ; // Retourne la vitesse du vent (en m/s)
            
        protected :
            void CollectDataFromTx23(void); // Récupère les données depuis la Tx23
            void ShowAllBits( char *Datas ) ; // Affiche les bites reçus vers la console

        private :
            void Tx23Delay(void );
            void DecodeWindir( char *Datas );
            void DecodeWinSpeed( char *Datas );

        private :
            uint8_t m_ui_pinIn ;
            float   m_fWindDir ; // en °C
            float   m_fWindSpeed ; // en m/s
            boolean m_bReadAskWindDir ;      // Drapeau servant à savoir si une interrogation de la Tx23 est nécessaire
            boolean m_bReadAskWindSpeed ; // Drapeau servant à savoir si une interrogation de la Tx23 est nécessaire
    };

#endif