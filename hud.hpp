class hud
{
    public:
        obiekt * gracz;
        bonus * bonusik;
        sf::Texture tex_paskaHp[ 2 ];
        sf::Texture tex_paskaBonusu[ 2 ];
        sf::Sprite spr_paskaHp;
        sf::Sprite spr_paskaBonusu;
        sf::Texture tex_tla;
        sf::Font czcionka;
        sf::Text tekst;
        sf::String tresc;
        sf::View kamera;
        int typ_bonusu;
        double timer[ 2 ];
        double alphaPaskaBonusu;

        hud()
        {
            tex_paskaHp[ 0 ].loadFromFile( "gfx/pasek_hpTlo.png" );
            tex_paskaHp[ 1 ].loadFromFile( "gfx/pasek_hp.png" );
            tex_paskaBonusu[ 0 ].loadFromFile( "gfx/pasek_bonusTlo.png" );
            tex_paskaBonusu[ 1 ].loadFromFile( "gfx/pasek_bonus.png" );
            spr_paskaHp.setTexture( tex_paskaHp[ 0 ] );
            spr_paskaBonusu.setTexture( tex_paskaBonusu[ 0 ] );
            tex_tla.loadFromFile( "gfx/tlo0.png" );
            czcionka.loadFromFile( "font/Barme Reczny.ttf" );
            tekst.setFont( czcionka );
            tekst.setCharacterSize( 18 );
            timer[ 0 ] = 0;
            timer[ 1 ] = 0;
            typ_bonusu = 0;
            alphaPaskaBonusu = 0.0001;
//            spr_paskaBonusu.setColor( sf::Color( 255, 255, 255, alphaPaskaBonusu ) );
        }

        void ustaw_timer( int ktory, int czas ){
            timer[ ktory ] = czas * 120;
        }

        double zwroc_timer( int ktory ){
            return timer[ ktory ] / 120;
        }

        void aktualizuj_timer( int ktory ){
            if( timer[ ktory ] > 0 )
                --timer[ ktory ];
            else
            {
//                if( ktory == 0 )
//                    bonusik->typ = -1;
            }
        }

        void pobierz_adresObiektu( obiekt * adres )
        {
            gracz = adres;
        }

        void pobierz_adresBonusu( bonus * adres )
        {
            bonusik = adres;
        }

        void ustaw_pozycjePaskaHp( double x, double y )
        {
            spr_paskaHp.setPosition( x, y );
        }

        void ustaw_pozycjePaskaBonusu( double x, double y )
        {
            spr_paskaBonusu.setPosition( x, y );
        }

//        void ustaw_rozmiarPaska( float h )
//        {
//            tlo_hp.setSize( sf::Vector2f( (float)(gracz->zwroc_maxHP() / gracz->zwroc_maxHP() * 100)+4, h+4 ) );
//            hp_gracza.setSize( sf::Vector2f(  gracz->zwroc_hp() / gracz->zwroc_maxHP() * 100 , h ) );
//        }

//        void ustaw_kolorPaska( sf::Color bg, sf::Color fg )
//        {
//            tlo_hp.setFillColor( bg );
//            hp_gracza.setFillColor( fg );
//        }

        void ustaw_pasek( double x, double y, float h )
        {
            ustaw_pozycjePaskaHp( x, y );
//            ustaw_rozmiarPaska( 20 );
//            ustaw_kolorPaska( bg, fg );
        }

        string konwersja( int liczba )
        {
            ostringstream skonwertowana_liczba;
            skonwertowana_liczba << liczba;
            string str = skonwertowana_liczba.str();
            return str;
        }

        void rysuj_tlo( sf::RenderWindow & okno )
        {

        }

        void rysuj_hud( sf::RenderWindow & okno )
        {
            if( bonusik->zwroc_timer() > 0 )
                alphaPaskaBonusu = 1;
//               else
//               if( bonusik->zwroc_timer() == 0 )
//                   alphaPaskaBonusu = 0;

            if( bonusik->zwroc_timer() == 0 ){
                if( alphaPaskaBonusu > 0 )
                    alphaPaskaBonusu -= 0.01;
            }
            spr_paskaBonusu.setColor( sf::Color( 255, 255, 255, 255*alphaPaskaBonusu));
            /// GDY HP DOJDZIE DO STANU KRYTYCZNEGO, ZAMIEN NA CZERWONY KOLOR
//            if( gracz->zwroc_hp() < 20 )
//                spr_paskaHp.setColor( sf::Color( 255, 255, 255, 255 ) );
//            else
//                spr_paskaHp.setColor( sf::Color( 255, 255, 255, 255 ) );
//            gracz->ustaw_hp( 10 );
            for( int i = 0 ; i < 2 ; i++ )
            {
                spr_paskaHp.setTexture( tex_paskaHp[ i ] );
                spr_paskaBonusu.setTexture( tex_paskaBonusu[ i ] );
//                spr_paskaBonusu.setScale( 1.5, 1 );
                if( i == 0 )
                {
                    spr_paskaHp.setTextureRect( sf::IntRect( 0, 0, 153, 18 ) );
                    spr_paskaBonusu.setTextureRect( sf::IntRect( 0, 0, 578, 31 ) );
                }
                else
                {
                    spr_paskaHp.setTextureRect( sf::IntRect( 0, 0,
                            (gracz->zwroc_hp()/gracz->zwroc_maxHP())*153, 18 ) );
                    spr_paskaBonusu.setTextureRect( sf::IntRect( 0, 0,
                            (bonusik->zwroc_timer() / bonusik->zwroc_maxTimer() )*578, 31 ) );
                }

                okno.draw( spr_paskaHp );
                if( alphaPaskaBonusu > 0 )
                {
                    okno.draw( spr_paskaBonusu );
                }
            }
            rysuj_tekst( okno, "Poziom: ", ::lvl, 170, 5 );
//            rysuj_tekst( okno, "Score: ", ::score );
            rysuj_tekst( okno, L"Pozostało: ", ::potrzeba_zabitych - ::ilosc_zabitych, 520, 110 );
            /// rysuj jaki lvl
            /// rysuj ile punktow
            /// rysuj jaki bonus
            if( bonusik->x != -500 && bonusik->typ == -1 )
                rysuj_tekst( okno,
                L"Szybko, bonus jest na mapie\n\tmusisz go znaleźć!", 200, 100 );
            if( bonusik->zwroc_timer() > 0 )
                rysuj_tekst( okno, "Czas bonusu: ", bonusik->zwroc_timer() + 1, 285, 410 );

            if( bonusik->typ != -1 )
            {
                ustaw_timer( 1, 2 );
                typ_bonusu = bonusik->typ;
            }
            if( zwroc_timer( 1 ) != 0 )
            {
                switch( typ_bonusu )
                {
                    case bonus_dmgUp:
                        rysuj_tekst( okno, L"Wylosowałes bonus: Zwiększenie ataku!", 200, 100 );
                        break;
                    case bonus_hpUp:
                        rysuj_tekst( okno, L"Wylosowałes bonus: Całkowite wyleczenie!", 200, 100 );
                        break;
                    case bonus_spdUp:
                        rysuj_tekst( okno, L"Wylosowałes bonus: Przyśpieszenie!", 200, 100 );
                        break;
                    case bonus_hpDown:
                        rysuj_tekst( okno, L"Co, ale że co?! HP ci spadło !", 200, 100 );
                        break;
                    case bonus_spdDown:
                        rysuj_tekst( okno, L"Chodzisz powooooli jak zółw...", 200, 100 );
                        break;
                    case bonus_immune:
                        rysuj_tekst( okno, L"Muahahaha! Nieśmiertelny! (czasowo ^^)", 200, 100 );
                        break;
                    case 6:
                        rysuj_tekst( okno, L"Nięęęęęę", 300, 10 );
                        break;
                    case -1: break;
                    default:
                        rysuj_tekst( okno, L"Nięę", 300, 10 );
                        break;
                }
            }
        }

        double zwroc_hpGracza() const{
            return gracz->zwroc_hp();
        }

        void rysuj_tekst( sf::RenderWindow & okno, sf::String _tresc,  double liczba, float x, float y )
        {
            double kamera_xHud = kamera.getCenter().x - kamera.getSize().x/2;
            double kamera_yHud = kamera.getCenter().y - kamera.getSize().y/2;
            tekst.setString( _tresc + konwersja( liczba ) );
            tekst.setPosition( kamera_xHud + x, kamera_yHud + y );
            okno.draw( tekst );
        }

        void rysuj_tekst( sf::RenderWindow & okno, sf::String _tresc, float x, float y )
        {
            double kamera_xHud = kamera.getCenter().x - kamera.getSize().x/2;
            double kamera_yHud = kamera.getCenter().y - kamera.getSize().y/2;
            tekst.setString( _tresc );
            tekst.setPosition( kamera_xHud + x, kamera_yHud + y );
            okno.draw( tekst );
        }

        void rysuj_tekst( sf::RenderWindow & okno, sf::Color kolor, sf::String _tresc, float x, float y )
        {
            tekst.setString( _tresc );
            tekst.setColor( kolor );
            tekst.setPosition( x, y );
            okno.draw( tekst );
            tekst.setColor( sf::Color::White );
        }


        void aktualizuj_kamere()
        {
            kamera.setCenter( gracz->zwroc_wsp().x, gracz->zwroc_wsp().y);
//            if( gracz->dmg_krytyczny ){
//                ::przyblizenie = 0.9;
//                gracz->dmg_krytyczny = false;
//            }

//            if( przyblizenie < 1 )
//                przyblizenie += 0.00000000000001;
//            if( przyblizenie > 1 )
//                przyblizenie = 1;

            kamera.zoom( przyblizenie );
        }

        void aktualizuj_hud()
        {
            aktualizuj_kamere();
            double kamera_xHud = kamera.getCenter().x - kamera.getSize().x/2;
            double kamera_yHud = kamera.getCenter().y - kamera.getSize().y/2;
            ustaw_pozycjePaskaHp( kamera_xHud+10 , kamera_yHud + 5 );
            ustaw_pozycjePaskaBonusu( kamera_xHud+30 , kamera_yHud + 440 );
            aktualizuj_timer( 1 );
        }

        void sprawdz_bonus()
        {
            bonusik->sprawdz_bonus();
        }

        void credits( sf::RenderWindow & okno )
        {
            sf::RectangleShape tlo_credits;
            tlo_credits.setFillColor( sf::Color::Black );
            tlo_credits.setPosition( kamera.getCenter().x - kamera.getSize().x/2,
                                     kamera.getCenter().y - kamera.getSize().y/2 );
            tlo_credits.setSize( sf::Vector2f( 640, 480 ) );
            okno.draw( tlo_credits );
            rysuj_tekst( okno, "Square War Alpha 0.9", 320, 20 );
            rysuj_tekst( okno, "Autorzy:\nOlek Szatko\nMarcin Kula\nRafal Kot", 320, 240 );
        }
};
//const unsigned int bonus_dmgUp   = 0;
//const unsigned int bonus_hpUp    = 1;
//const unsigned int bonus_spdUp   = 2;
//const unsigned int bonus_hpDown  = 3;
//const unsigned int bonus_spdDown = 4;
//const unsigned int bonus_immune  = 5;

