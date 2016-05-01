class obiekt{
    private:
        double hp, max_hp;
        double min_dmg, max_dmg, dmg;
        sf::Texture tex;
        sf::RectangleShape pasek_hp, pasek_ataku;
        string nazwa;
        sf::Vector2f wsp;
        double spd_x, spd_y, spd_maxX, spd_maxY;
        bool stan[ 6 ];
        double timer, max_timer;

    public:
        static obiekt* *lista;
        sf::Sprite spr;
        static int rzeczywista_ilObiektow;
        int id;
        double dir;
        bool dmg_krytyczny;

        obiekt( string nazwa = "", double _x = 0 , double _y = 0,
                double spd = 0, double _hp = 0, double dmg = 0 )
        {
            spr.setOrigin( 16, 16 );
            for( int i = 0 ; i < MOZE_ATAKOWAC ; i++ )
            {
                stan[ i ] = 0;
            }
            stan[ ZYJE ] = true;
            stan[ MOZE_ATAKOWAC ] = true;
            ustaw_maxTimer( 0.5 );
            timer = 0;
            id = rzeczywista_ilObiektow;
            rzeczywista_ilObiektow++;
            dir = 0;
            dmg_krytyczny = false;
        }

        virtual ~obiekt()
        {

        }

        void zmien_wsp( double _x, double _y ){
            wsp.x = _x;
            wsp.y = _y;
        }

        void ustaw_minDmg( double _minDmg ){
            min_dmg = _minDmg;
        }

        void ustaw_maxDmg( double _maxDmg ){
            max_dmg = _maxDmg;
        }

        void ustaw_spdX( double _spd_x ){
            spd_x = _spd_x;
            spd_maxX = _spd_x;
        }

        void ustaw_spdY( double _spd_y ){
            spd_y = _spd_y;
            spd_maxY = _spd_y;
        }

        void ustaw_hp( double _hp ){
            hp = _hp;
        }

        void ustaw_maxHP( double _max_hp ){
            max_hp = _max_hp;
        }

        void zmien_nazwe( string _nazwa ){
            nazwa = _nazwa;
        }

        void ustaw_pasekHP( double _size )
        {
            pasek_hp.setFillColor( sf::Color::Red );
            pasek_hp.setSize( sf::Vector2f( _size, 5 ) );
        }

        void zmien_wspPaskaHp( double _x, double _y )
        {
            pasek_hp.setPosition( _x, _y );
        }

        void ustaw_wielkoscPaska( double _size ){
            pasek_hp.setSize( sf::Vector2f( _size, 5 ) );
        }

        void ustaw_pozPoczatkowa( float _x, float _y )
        {
            wsp.x = _x;
            wsp.y = _y;
        }

        void ustaw_timer( double _timer )
        {
            timer = _timer * 120;
        }

        void ustaw_maxTimer( double _max_timer )
        {
            max_timer = _max_timer * 120;
        }

        bool gracz_uderz(){
            if( !stan[ MOZE_ATAKOWAC ] || !stan[ ZYJE ] )
                return 0;
            for( int i = 0 ; i < ilosc_obiektow ; i++ )
            {
                if( wsp.x != lista[ i ]->wsp.x && wsp.y != lista[ i ]->wsp.y
                   && kolizja( wsp.x, wsp.y, lista[ i ]->wsp.x, lista[ i ]->wsp.y ) )
                {
                    if( sf::Keyboard::isKeyPressed( sf::Keyboard::J ) )
                    {
                        dmg = losuj( min_dmg, max_dmg );
                        lista[ i ]->ustaw_hp( lista[ i ]->zwroc_hp()-dmg );
                        if( dmg > 60 )
                            dmg_krytyczny = true;
//                        cout << "lista[ " << i << " ] = " << lista[ i ]->zwroc_hp() << endl;
                        ustaw_timer( zwroc_maxTimer() ); /// Pseudo 2s.
                        stan[ MOZE_ATAKOWAC ] = false;
                        if( wsp.x+16 > lista[ i ]->wsp.x-16 ){
                            lista[ i ]->wsp.x -= wsp.x - lista[ i ]->wsp.x + 16 ;
                        }
                        else
                        if( wsp.x-16 < lista[ i ]->wsp.x+16 ){
                            lista[ i ]->wsp.x += lista[ i ]->wsp.x - wsp.x - 16 ;
                        }

                        if( wsp.y+16 > lista[ i ]->wsp.y ){
                            lista[ i ]->wsp.y -= wsp.y - lista[ i ]->wsp.y - 16 ;
                        }
                        else
                        if( wsp.y-16 < lista[ i ]->wsp.y+16 ){
                            lista[ i ]->wsp.y += lista[ i ]->wsp.y - wsp.y + 16 ;
                        }
                        lista[ i ]->spd_x = -2;
                        lista[ i ]->spd_y = -2;
                    }
                }
            }
            return 1;
        }

        bool bot_uderz(){
            if( !stan[ MOZE_ATAKOWAC ] || ::ilosc_zabitych >= ::potrzeba_zabitych )
                return 0;
                if( kolizja( wsp.x, wsp.y, lista[ ilosc_obiektow - 1 ]->wsp.x,
                            lista[ ilosc_obiektow - 1 ]->wsp.y ) )
                {
                        lista[ ilosc_obiektow - 1 ]->
                            ustaw_hp( lista[ ilosc_obiektow - 1 ]->zwroc_hp()-min_dmg );

//                        cout << "lista[ " << i << " ] = " << lista[ i ]->zwroc_hp() << endl;
                        ustaw_timer( 3.0 ); /// Pseudo 3s.
                        stan[ MOZE_ATAKOWAC ] = false;
                }
            return 1;
        }

        void ustaw_gfx( string sciezka ){
            tex.loadFromFile( "gfx/" + sciezka );
            spr.setTexture( tex );
        }

        bool kolizja( double x1, double y1, double x2, double y2 ) const
        {
            if( x1+SPR_W/2 > x2-SPR_W/2 &&
                x1-SPR_W/2 < x2+SPR_W/2 &&
                y1+SPR_H/2 > y2-SPR_H/2 &&
                y1-SPR_H/2 < y2+SPR_H/2 )
                return 1;
            else
                return 0;
        }

        bool kolizja_okno(){
            if( wsp.x+SPR_W/2 > OKNO_W ) wsp.x = OKNO_W - SPR_W/2;
            else
            if( wsp.x-SPR_W/2 < 0 ) wsp.x = 0 + SPR_W/2;

            if( wsp.y+SPR_H/2 > OKNO_H ) wsp.y = OKNO_H - SPR_H/2;
            else
            if( wsp.y-SPR_H/2 < 0 ) wsp.y = 0 + SPR_H/2;
        }

        sf::Vector2f zwroc_wsp() const{
            return wsp;
        }

        double zwroc_dmg() const{
            return dmg;
        }

        double zwroc_maxDmg() const{
            return max_dmg;
        }

        double zwroc_hp() const{
            return hp;
        }

        double zwroc_maxHP() const{
            return max_hp;
        }

        sf::Sprite zwroc_spr(){
            return spr;
        }

        bool zwroc_stan( int i ) const{
            return stan[ i ];
        }

        double zwroc_timer() const{
            return timer / 120;
        }

        double zwroc_maxTimer() const{
            return max_timer / 120;
        }

        string zwroc_nazwe() const{
            return nazwa;
        }

        sf::RectangleShape zwroc_pasekHP(){
            return pasek_hp;
        }

        double aktualizuj_timer(){
            if( timer > 0 )
            {
               --timer;
               dir += 360/zwroc_timer();
               spr.setRotation( dir );
            }
            else
            {
                dir = 0;
                spr.setRotation( dir );
                stan[ MOZE_ATAKOWAC ] = true;
            }
        }

        void aktualizuj_pasekHP(){
            pasek_hp.setSize( sf::Vector2f( ((hp / max_hp) * 100)/3.3, 5 ) );
            pasek_hp.setPosition( wsp.x - 15, wsp.y - 20 );
        }

        void steruj()
        {
            if(  !stan[ ZYJE ] )
                return ;

            float nx = wsp.x, ny = wsp.y;
            bool _kol = 0;

            if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
                ny -= spd_y;
            else
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
                ny += spd_y;

            if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
                nx -= spd_x;
            else
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
                nx += spd_x;

                wsp.x = nx;
                wsp.y = ny;
        }

//        void steruj()
//        {
//            if(  !stan[ ZYJE ] )
//                return ;
//
//            auto float nx = wsp.x, ny = wsp.y;
//            auto bool _kol = 0;
//
//            if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) ){
//                spd_y -= 0.1;
//            }
//
//            else
//            if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) ){
//                spd_y += 0.1;
//            }
//
//
//            if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ){
//                if( spd_x > -spd_maxX )
//                    spd_x -= 0.1;
//                else
//                    spd_x = -spd_maxX;
//            }
//
//            else
//            if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ){
//                if( spd_x < spd_maxX )
//                    spd_x += 0.1;
//                else
//                    spd_x = spd_maxX;
//            }
//                nx += spd_x;
//                ny += spd_y;
//                wsp.x = nx;
//                wsp.y = ny;
//        }

//
        void ruch()
        {
            if( !stan[ ZYJE ] || ::ilosc_zabitych >= ::potrzeba_zabitych
                || !lista[ ilosc_obiektow - 1 ]->stan[ ZYJE ] )
                return ;

            float gracz_x = lista[ ilosc_obiektow - 1 ]->zwroc_wsp().x;
            float gracz_y = lista[ ilosc_obiektow - 1 ]->zwroc_wsp().y;

            if( stan[ MOZE_ATAKOWAC ] )
            {
                if( spd_x < 0 )
                    spd_x = -spd_x;
                if( spd_y < 0 )
                    spd_y = -spd_y;
            }
            else
            {
                if( spd_x > 0 )
                    spd_x = -1;
                if( spd_y < 0 )
                    spd_y = -1;
            }

            if( !kolizja( wsp.x, wsp.y, gracz_x, gracz_y ) )
            {
                stan[ RUCH ] = true;
                if( gracz_x > wsp.x )
                    wsp.x += spd_x;
                else
                if( gracz_x < wsp.x )
                    wsp.x -= spd_x;

                if( gracz_y > wsp.y )
                    wsp.y += spd_y;

                if( gracz_y < wsp.y )
                    wsp.y -= spd_y;
            }
            else
            {
                stan[ RUCH ] = false;
                bot_uderz();
            }
        }

        void aktualizuj_spd(){
            if( spd_x < spd_maxX ) spd_x += 0.03;
            else
            if( spd_y < spd_maxY ) spd_y += 0.03;

            if( spd_x > spd_maxX ) spd_x = spd_maxX;
            else
            if( spd_y > spd_maxY ) spd_y = spd_maxY;
        }

        void losuj_pozycje(){
            double _x, _y;
            do{
                _x = rand();
                _y = rand();

                if( !kolizja( _x, _y, lista[ ilosc_obiektow - 1 ]->wsp.x, lista[ ilosc_obiektow - 1]->wsp.y ) )
                    break;
            }while( 1 );

        }

        void aktualizuj_Bothp(){
            if( hp <= 0 )
            {
                ustaw_hp( 0 );
                stan[ ZYJE ] = false;
                id_obiektu = id;
                ::czy_bonus =  3 ;
                cout << "czy_bonus = " << ::czy_bonus << endl;
            }
        }

        void aktualizuj_stanZyje(){
                if( ::ilosc_zabitych < ::potrzeba_zabitych &&
                    hp <= 0 )
                {
                    ::ilosc_zabitych++;
                    stan[ ZYJE ] = true;
                    max_hp += 5;
                    spd_maxX += 0.1;
                    spd_maxY += 0.1;
                    spd_x = spd_maxX;
                    spd_y = spd_maxY;
                    spr.setColor( sf::Color::Magenta );
                    ustaw_hp( max_hp );
                    lista[ ilosc_obiektow - 1 ]->hp += 5;
                    wsp.x =  losuj( 0, 1024 );
                    wsp.y = losuj( 0, 768 );
                    /// Opcjonalnie znow moze atakowac po zabiciu bota
                    /// gracz wbija lvl, a wraz z nim zwieksza swoje statystyki
                }

            if( hp > 0 )
            {
                if( !stan[ ZYJE ] )
                    stan[ ZYJE ] = true;
            }
        }

        void ustaw_stan( int ktory, int ustaw_na )
        {
            stan[ ktory ] = ustaw_na;
        }

        void ustaw_pasekAtaku(  double x, double y, float h, sf::Color bg )
        {
            pasek_ataku.setPosition( x, y );
            pasek_ataku.setFillColor( bg );
            pasek_ataku.setSize( sf::Vector2f( (timer / max_timer * 100)/3.3, h ) );
        }

        sf::RectangleShape zwroc_pasekAtaku() const{
            return pasek_ataku;
        }

        void ustaw_rozmiarPaskaAtaku( float h )
        {
            pasek_ataku.setSize( sf::Vector2f( ( zwroc_timer() / zwroc_maxTimer() )*100/3.3, h ) );
        }

        void ustaw_pozycjePaskaAtaku( double x, double y ){
            pasek_ataku.setPosition( x, y );
        }

        int zwroc_id(){
            return id;
        }

};

obiekt* *obiekt::lista = NULL;
int obiekt::rzeczywista_ilObiektow = 0;
