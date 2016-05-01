class bonus
{
    private:
        sf::Texture tex;
    public:
        sf::Sprite spr;
        double x, y;
        int typ;
        static obiekt * gracz;
        static obiekt* *bot;
        static bool sprawdzany;
        double timer;
        double max_timer;

        bonus( obiekt * _gracz ){
            x = - 500;
            y = - 500;
            typ = -1;
            tex.loadFromFile( "gfx/bonus.png" );
            spr.setTexture( tex );
            spr.setPosition( x, y );
            gracz = _gracz;
            timer = 0;
            max_timer = 1;
        }

        ~bonus(){
            tex.~Texture();
            spr.~Transformable();
            spr.~Drawable();
        }

        void ustaw_timer( int na_ile ){
            timer = na_ile*120;
        }

        double zwroc_timer(){
            return timer / 120;
        }

        void ustaw_maxTimer( int na_ile ){
            max_timer = na_ile*120;
        }

        double zwroc_maxTimer(){
            return max_timer / 120;
        }

        void aktualizuj_timer(){
            if( timer > 0 )
                --timer;
            else
            {
                switch( typ )
                {
                    case bonus_dmgUp:
                        gracz->ustaw_minDmg( 50 );
                        break;

                    case bonus_spdUp:
                        gracz->ustaw_spdX( 3 );
                        gracz->ustaw_spdY( 3 );
                        break;

                    case bonus_spdDown:
                        gracz->ustaw_spdX( 3 );
                        gracz->ustaw_spdY( 3 );
                        break;

                    case bonus_immune:
                        break;

                }//switch
                typ = -1;
            }
        }

        int losuj_bonus( int _od, int _do ){
                typ = ( rand() % _do ) + _od;
            return typ;
        }

        void sprawdz_bonus(){
            if( typ == -1 )
                return ;
//            if( sprawdzany )
//                return ;

            switch( typ )
            {
                case bonus_dmgUp:
                    gracz->ustaw_minDmg( 80 );
                    break;

                case bonus_hpUp:
                    gracz->ustaw_hp( gracz->zwroc_maxHP() );
                    break;

                case bonus_spdUp:
                    gracz->ustaw_spdX( 5 );
                    gracz->ustaw_spdY( 5 );
                    break;

                case bonus_hpDown:
                    gracz->ustaw_hp( gracz->zwroc_hp() - ( gracz->zwroc_hp() / gracz->zwroc_maxHP()*5) );
                    break;

                case bonus_spdDown:
                    gracz->ustaw_spdX( 2 );
                    gracz->ustaw_spdY( 2 );
                    break;

                case bonus_immune:
                    break;

            }//switch
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

        bool kolizjaGracz()
        {
            if( !(x+SPR_W/2 > gracz->zwroc_wsp().x-SPR_W/2 &&
                x-SPR_W/2 < gracz->zwroc_wsp().x+SPR_W/2 &&
                y+SPR_H/2 > gracz->zwroc_wsp().y-SPR_H/2 &&
                y-SPR_H/2 < gracz->zwroc_wsp().y+SPR_H/2) )
            {
                aktualizuj_timer();
                if( ::czy_bonus  == 3 )
                {
                    if( sprawdzany == false )
                    {
                        x = losuj( 0, 1024);
                        y = losuj( 0, 768 );
                        sprawdzany = true;
                    }
                    spr.setPosition( x, y );
                }

            }
            else
            {
                if( ::czy_bonus == 3 )
                {
                    sprawdzany = false;
                    ustaw_timer( 0 );
                    aktualizuj_timer();
                    losuj_bonus( 0, 6 );
                    sprawdz_bonus();
                    if( typ != bonus_hpUp && typ != bonus_hpDown )
                        ustaw_timer( losuj( 4, 12 ) );
                    ustaw_maxTimer( zwroc_timer() );
                    x = -500;
                    y = -500;
                    spr.setPosition( x, y );
                    ::czy_bonus = 0;
                }
            }
        }

};

obiekt * bonus::gracz = NULL;
obiekt* *bonus::bot = NULL;
bool bonus::sprawdzany = false;
