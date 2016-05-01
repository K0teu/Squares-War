sf::Texture tex_efektow[ 2 ];

/**
**************************************************************************
****************************** JAK UZYWAC ********************************
**************************************************************************
/
/   Tworzymy obiekt klasy efekt np. efektyGraficzne moj_efekt
/   Tworzymy efekt np. moj_efekt.stworzEfekt(...);
/   W zaleznosci, czy chcemy efekt graficzny, czy tekstowy
/       wybieramy odpowiednie przeciazenie tej metody
/   argumenty ( (sf::Texture) tekstura / (string) tekst,
/                (int) tablica kolorow[4],
/                (bool) flaga mowiaca czy ten kolor ma byc zmieniany samoistnie,
/                (double) przezroczystosc(od 0 do 1),
/                (bool) flaga mowiaca czy przezroczystosc ma sie zmieniac samoistnie,
/                (double) predkosc zmiany przezroczystosci,
/                (double) x efektu, (double) y efektu,
/                (double) kat pod ktorym ma byc obrocony efekt,
/                (double) predkosc rotacji efektu,
/                (bool) flaga mowiaca czy kat ma sie zmieniac samoistnie,
/                (double) skala wielkosci sprajta efektu / wielkosc czcionki,
/                (bool) flaga mowiaca czy skala wielkosci / wielkosc czcionki
                                ma sie zmieniac samoistnie,
/                (double) wektor ruchu w poziomie,
/                (double) wektor ruchu w pionie,
/                (int) Origin x, (int)Origin Y )
/
/ Nastepnie aktualizujemy calosc efektu np. moj_efekt.aktualizujEfekt()
/ Wazne, zeby aktualizacja byla wczesniej niz samo rysowanie
/ Teraz pozostalo go tylko wyrysowac ( dajemy to zaraz po okno.clear )
/ nie dajemy tego wczesniej niz okno.clear, ale musimy pamietac
/ zeby dac wczesniej niz okno.display tak wiec np. moj_efekt.rysuj( okno );
/////////////////////////////////////////////////////////////////////////////
**/

class efektGraficzny
{
    private:
        sf::Color color;
        int colors[ 4 ];
        int max_ofColors[ 4 ];
        int colorChangeSpd;
        double alpha, alphaChangeSpd;
        double rotation, rotationSpd;
        double scale, maxScale;
        sf::Texture tex;
        double x, y, spdX, spdY;
        bool alphaChange, scaleChange, colorChange, rotationChange;
        bool alphaChanged, scaleChanged, colorChanged;
        int OriginX, OriginY;
        int effectType;
        sf::Font font;
        sf::Text text;

    public:
        sf::Sprite spr;

        efektGraficzny(){
            alphaChanged = 0;
            scaleChanged = 0;
            colorChanged = 0;
            color = sf::Color( 0, 0, 0, 0 );
            for( int i = 0; i < 4; i++ )
                max_ofColors[ i ] = 255;
            colorChange = 0;
            colorChangeSpd = 1;
            alpha = 0;
            alphaChange = 0;
            alphaChangeSpd = 0;
            x = 0;
            y = 0;
            rotation = 0;
            rotationSpd = 0;
            scale = 1;
            scaleChange = 0;
            spdX = 0;
            spdY = 0;
            effectType = -1;
        }
        ~efektGraficzny(){
            /// Zwolnienie pamieci
        }
        void stworzEfekt( sf::Texture & _tex,
                           int _color[],
                           bool _colorChange = 0,
                           double _alpha = 1, bool _alphaChange = 0,
                           double _alphaChangeSpd = 0, int _x = 0, int _y = 0,
                           double _rotation = 0, double _rotationSpd = 0,
                           bool _rotationChange = 0,
                           double _scale = 1,bool _scaleChange = 0,
                           double _spdX = 0, double _spdY = 0,
                           int _OriginX = 0, int _OriginY = 0
                            );

        void stworzEfekt( string _text,
                           int _color[],
                           bool _colorChange = 0,
                           double _alpha = 1, bool _alphaChange = 0,
                           double _alphaChangeSpd = 0, int _x = 0, int _y = 0,
                           double _rotation = 0, double _rotationSpd = 0,
                           bool _rotationChange = 0,
                           double _scale = 1,bool _scaleChange = 0,
                           double _spdX = 0, double _spdY = 0,
                           int _OriginX = 0, int _OriginY = 0
                            );

        void stworzEfekt( char type[] );

        bool koniec_animacji(){
            if( alphaChanged ||
                colorChanged ||
                scaleChanged )
                return true;
            else
                return false;
        }

        void rysuj( sf::RenderWindow & okno );

        void aktualizujEfekt();

        void ustaw_maxKolor( int _max_ofColors[] ){
            for( int i = 0; i < 4; i++ )
                max_ofColors[ i ] = _max_ofColors[ i ];
        }

        void ustaw_kolor( int _colors[] ){
            for( int i = 0; i < 4; i++ )
                colors[ i ] = _colors[ i ];
            color = sf::Color( colors[ 0 ], colors[ 1 ], colors[ 2 ], colors[ 3 ] );
        }

        void ustaw_przezroczystosc( int _alpha ){
            alpha = _alpha;
        }

        void ustaw_katObrotu( double _rotation ){
            rotation = _rotation;
        }

        void ustaw_skale( double _scale ){
            scale = _scale;
        }

        void ustaw_maxSkale( double _maxScale ){
            maxScale = _maxScale;
        }

        void ustaw_koordynaty( double _x, double _y ){
            x = _x;
            y = _y;
        }

        void ustaw_wektoryRuchu( double _spdX, double _spdY ){
            spdX = _spdX;
            spdY = _spdY;
        }

        void ustaw_srodek( int _originX, int _originY ){
            OriginX = _originX;
            OriginY = _originY;
        }

        void wczytaj_czcionke( string path ){
            font.loadFromFile( path );
            text.setFont( font );
        }

        void ustaw_tekst( string _text ){
            text.setString( _text );
        }

        void ustaw_predkoscZmianyPrzezroczystoci( double _alphaChangeSpd ){
            alphaChangeSpd = _alphaChangeSpd;
        }

        void ustaw_predkoscRotacji( double _rotationSpd ){
            rotationSpd = _rotationSpd;
        }

        void ustaw_flageKoloru( bool _colorChange ){
            colorChange = _colorChange;
        }

        void ustaw_flagePrzezroczystosci( bool _alphaChange ){
            alphaChange = _alphaChange;
        }

        void ustaw_flageSkalowania( bool _scaleChange ){
            scaleChange = _scaleChange;
        }

        void ustaw_predkoscZmianyKoloru( double _spd ){
            colorChangeSpd = _spd;
        }
};


void efektGraficzny::stworzEfekt( sf::Texture & _tex,
                int _color[], bool _colorChange,
                double _alpha, bool _alphaChange,
                double _alphaChangeSpd, int _x, int _y,
                double _rotation, double _rotationSpd,
                bool _rotationChange,
                double _scale, bool _scaleChange,
                double _spdX, double _spdY,
                int _OriginX, int _OriginY
                    )
{
    effectType = 0;
    tex.bind( &_tex );
    color = sf::Color( _color[ 0 ], _color[ 1 ], _color[ 2 ], 255 );
    for( int i = 0 ; i < 3 ; i++ )
        colors[ i ] = _color[ i ];
    colorChange = _colorChange;
    alpha = _alpha;
    alphaChange = _alphaChange;
    alphaChangeSpd = _alphaChangeSpd;
    x = _x;
    y = _y;
    rotation = _rotation;
    rotationSpd = _rotationSpd;
    rotationChange = _rotationChange;
    scale = _scale;
    scaleChange = _scaleChange;
    maxScale = 1;
    spdX = _spdX;
    spdY = _spdY;
    spr.setTexture( _tex );
    OriginX = _OriginX;
    OriginY = _OriginY;
    spr.setOrigin( OriginX, OriginY );
}

void efektGraficzny::stworzEfekt( string _text,
                int _color[], bool _colorChange,
                double _alpha, bool _alphaChange,
                double _alphaChangeSpd, int _x, int _y,
                double _rotation, double _rotationSpd,
                bool _rotationChange,
                double _charSize, bool _charSizeChange, double _spdX, double _spdY,
                int _OriginX, int _OriginY
                    )
{
    effectType = 1;
    text.setString( _text );
    color = sf::Color( _color[ 0 ], _color[ 1 ], _color[ 2 ], _color[ 3 ] );
    for( int i = 0 ; i < 4 ; i++ )
        colors[ i ] = _color[ i ];
    alpha = _alpha;
    text.setColor( color );
    alphaChange = _alphaChange;
    alphaChangeSpd = _alphaChangeSpd;
    x = _x;
    y = _y;
    rotation = _rotation;
    text.setRotation( rotation );
    rotationSpd = _rotationSpd;
    rotationChange = _rotationChange;
    scale = _charSize;
    maxScale = _charSize;
    text.setCharacterSize( scale );
    scaleChange = _charSizeChange;
    spdX = _spdX;
    spdY = _spdY;
    OriginX = _OriginX;
    OriginY = _OriginX;
    text.setOrigin( OriginX, OriginY );
}

//void efektGraficzny::stworzEfekt( char type[] )
//{
//    effectType = 2;
//    if( type == "swiatlo" )
//    {
//            colors[ 0 ] = 255;
//            colors[ 1 ] = 40;
//            colors[ 3 ] = 25;
//            colors[ 2 ] = 255;
//            stworzEfekt(
//                ::tex_efektow[ 0 ],
//                colors, 1,
//                1, 1,
//                0.2, 1, 0,
//                1, 6,
//                1, 1, 0, 2, 2,
//                81,61
//                );
//    }
//    else
//    if( type == "otchlanFull" )
//    {
//            colors[ 0 ] = 0;
//            colors[ 1 ] = 0;
//            colors[ 3 ] = 255;
//            colors[ 2 ] = 255;
//            stworzEfekt(
//                ::tex_efektow[ 1 ],
//                colors, 0,
//                1, 1,
//                0.01, 0, 0,
//                0, 0,
//                0, 1, 0, 0, 0,
//                0,0
//                );
//    }
//    else
//    if( type == "otchlan" )
//    {
//        colors[ 0 ] = 0;
//        colors[ 1 ] = 0;
//        colors[ 3 ] = 255;
//        colors[ 2 ] = 255;
//        stworzEfekt(
//            ::tex_efektow[ 1 ],
//            colors, 0,
//            0, 1,
//            0.01, 0, 0,
//            0, 0,
//            0, 10, 0, 0, 0,
//            0,0
//            );
//    }
//}

void efektGraficzny::aktualizujEfekt(){

    if( spdX != 0 )
        x += spdX;
    if( spdY != 0 )
        y += spdY;

    if( scaleChange ){

        if( scale <= 0 )
            scaleChanged = true;
        else
        if( scale >= maxScale )
            scaleChanged = false;

        if( scaleChanged == true )
            scale += 0.01;
        else
        if( scaleChanged == false )
            scale -= 0.01;

        if( effectType != -1 && effectType != 1 )
            spr.setScale( scale, scale );
        else
            text.setCharacterSize( scale );
    }

    if( colorChange ){
        if( colorChanged )
        {
            for( int i = 0; i < 3; i++ )
                colors[ i ]+=colorChangeSpd;
        }
        else
        {
            for( int i = 0; i < 3; i++ )
                colors[ i ]-=colorChangeSpd;
        }
        for( int i = 0; i < 3; i++ )
        {
            if( colors[ i ] >= max_ofColors[ i ] )
                colorChanged = !colorChanged;
        }
        color = sf::Color( colors[ 0 ], colors[ 1 ], colors[ 2 ], colors[ 3 ] );
    }

    if( alphaChange ){
        if( !alphaChanged )
        {
            if( alpha > 0 )
                alpha-= alphaChangeSpd;
        }
        else
        {
            if( alpha < 1 )
                alpha+= alphaChangeSpd;
        }
        if( alpha >= 1 )
            alphaChanged = false;
        else
        if( alpha <= 0 )
            alphaChanged = true;
        colors[ 3 ] = alpha*255;
        color = sf::Color( colors[ 0 ], colors[ 1 ], colors[ 2 ], colors[ 3 ] );
    }

    if( rotationChange ){
        rotation += rotationSpd;
        if( rotation == 360 ||
            rotation == -360 )
            rotation = 0;
    }

    if( effectType != -1 && effectType != 1 )
    {
        spr.setPosition( x, y );
        spr.setColor( color );
        spr.setScale( scale, scale );
        spr.setRotation( rotation );
    }
    else{
        text.setPosition( x, y );
        text.setColor( color );
        text.setCharacterSize( scale );
        text.setRotation( rotation );
    }
}

void efektGraficzny::rysuj( sf::RenderWindow & okno ){
    if( effectType != -1 && effectType != 1 )
        okno.draw( spr );
    else
        okno.draw( text );
}
