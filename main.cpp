#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

using namespace std;
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include "efektyGraficzne.h"

/// STANY OBIEKTOW
#define KOLIZJA       0
#define UDERZONY      1
#define RUCH          2
#define CZY_BONUS     3
#define MOZE_ATAKOWAC 4
#define ZYJE          5

#define DZWIEK_BONUS             0
#define DZWIEK_ATAK              1
#define DZWIEK_MALO_HP           2
#define DZWIEK_MENU_ZMIANA_OPCJA 3
#define DZWIEK_MENU_WYBOR_OPCJI  4

#define MENU_GRAJ       0
#define MENU_USTAWIENIA 1
#define MENU_AUTORZY    2
#define MENU_WYJDZ      3

const unsigned int bonus_dmgUp   = 0;
const unsigned int bonus_hpUp    = 1;
const unsigned int bonus_spdUp   = 2;
const unsigned int bonus_hpDown  = 3;
const unsigned int bonus_spdDown = 4;
const unsigned int bonus_immune  = 5;

/// USTALENIA OGOLNE
#define OKNO_W 1024
#define OKNO_H 768
#define SPR_W   32
#define SPR_H   32

const unsigned int poczatkowaIlosc_obiektow = 3;
unsigned int ilosc_obiektow = poczatkowaIlosc_obiektow;
const unsigned int maxIlosc_obiektow = 45;
int lvl = 1, ilosc_zabitych = 0, potrzeba_zabitych = 5;
bool pauza = false;
bool przeszedl_lvl = false;
double hp_gracza = 100;
double maxHp_gracza = 100;
float przyblizenie = 1;

int id_obiektu = -1;

int losuj( int _od, int _do ){
    int wylosowana = ( rand() % _do )+ _od;
    cout << "Wylosowana liczba to: " << wylosowana << endl;
    return wylosowana;
}

sf::RectangleShape zaslona;
sf::Time czas_zaslony;

int czy_bonus = 0;

#include "obiekt.hpp"
#include "bonus.hpp"
#include "hud.hpp"

obiekt * wsk[ maxIlosc_obiektow ];
hud interfejs;
bonus bonus0( wsk[ ilosc_obiektow - 1 ] );

void komunikat( sf::RenderWindow & okno, string tytul, string tresc, char typ )
{
    HWND uchwyt = okno.getSystemHandle();
    if( typ != 'e' )
        MessageBox( uchwyt, tresc.c_str(), tytul.c_str(), MB_ICONINFORMATION | MB_OK | MB_TASKMODAL );
    else
        MessageBox( uchwyt, tresc.c_str(), tytul.c_str(), MB_ICONERROR | MB_OK | MB_TASKMODAL );
}

void inicjalizuj()
{
    for( int i = 0 ; i < ilosc_obiektow - 1 ; i++ )
    {
        wsk[ i ] = new obiekt;
        wsk[ i ]->ustaw_maxHP( 50+((i*lvl)*2) );
        wsk[ i ]->ustaw_hp( wsk[ i ]->zwroc_maxHP() );
        wsk[ i ]->zmien_wsp( losuj( 0, 1024 ), losuj( 0, 768 ) );
        wsk[ i ]->ustaw_minDmg( 1*lvl );
        wsk[ i ]->ustaw_spdX( i*0.1+0.2 );
        wsk[ i ]->ustaw_spdY( i*0.1+0.2 );
        wsk[ i ]->ustaw_gfx( "bot_a.png" );
        wsk[ i ]->ustaw_pasekHP(
                ( (wsk[ i ]->zwroc_hp() / wsk[ i ]->zwroc_maxHP()) *100)/3.3 );
        wsk[ i ]->zmien_nazwe( "BOT_" );
        wsk[ i ]->ustaw_timer( 0 );
    }

    wsk[ ilosc_obiektow - 1 ] = new obiekt;
    wsk[ ilosc_obiektow - 1 ]->ustaw_maxHP( maxHp_gracza );
    wsk[ ilosc_obiektow - 1 ]->ustaw_hp( hp_gracza );
    wsk[ ilosc_obiektow - 1 ]->zmien_wsp( 1024/2, 768/2 );
    wsk[ ilosc_obiektow - 1 ]->ustaw_minDmg( 50 );
    wsk[ ilosc_obiektow - 1 ]->ustaw_maxDmg( 80 );
    wsk[ ilosc_obiektow - 1 ]->ustaw_spdX( 3 );
    wsk[ ilosc_obiektow - 1 ]->ustaw_spdY( 3 );
    wsk[ ilosc_obiektow - 1 ]->ustaw_gfx( "gracz.png" );
    wsk[ ilosc_obiektow - 1 ]->ustaw_pasekHP(
                                ( (wsk[ ilosc_obiektow - 1 ]->zwroc_hp() /
    wsk[ ilosc_obiektow - 1 ]->zwroc_maxHP()) *100)/3.3 );
    wsk[ ilosc_obiektow - 1 ]->zmien_nazwe( "GRACZ" );
    wsk[ ilosc_obiektow - 1 ]->ustaw_timer( 0 );
    wsk[ ilosc_obiektow - 1 ]->ustaw_pasekAtaku(  wsk[ ilosc_obiektow - 1 ]->zwroc_wsp().x, wsk[ ilosc_obiektow - 1 ]->zwroc_wsp().y+20, 5, sf::Color::White );
    interfejs.pobierz_adresObiektu( wsk[ ilosc_obiektow - 1 ] );
    interfejs.bonusik->gracz = wsk[ ilosc_obiektow - 1 ];
//    interfejs.ustaw_pasek( 10, 10, 20, sf::Color::Red, sf::Color::Green );

}

void nastepny_poziom( sf::RenderWindow & okno ){
    ++lvl;

    if( ilosc_obiektow < maxIlosc_obiektow )
        ilosc_obiektow += 2;

    if( ilosc_obiektow > maxIlosc_obiektow )
        ilosc_obiektow = maxIlosc_obiektow;

    ilosc_zabitych = 0;
    potrzeba_zabitych = 3*lvl;

    hp_gracza = interfejs.gracz->zwroc_hp();
    delete [] wsk;
    interfejs.bonusik->x = -500;
    interfejs.bonusik->y = -500;
    interfejs.bonusik->spr.setPosition( -500, -500 );
    ::czy_bonus = 0;
    bonus::sprawdzany = false;
    bonus0.ustaw_timer( 0 );
    ::maxHp_gracza += 5;

    if( ::hp_gracza + 20 < ::maxHp_gracza )
        ::hp_gracza += 20;
    else
        ::hp_gracza = ::maxHp_gracza;

    inicjalizuj();
    zaslona.setPosition(
        interfejs.kamera.getCenter().x - interfejs.kamera.getSize().x/2,
        interfejs.kamera.getCenter().y - interfejs.kamera.getSize().y/2);
    okno.draw( zaslona );
    interfejs.rysuj_tekst( okno, L"Gratulacje, udało ci się przejść do kolejnego poziomu!", 100, 220 );
    okno.display();
    sf::sleep( czas_zaslony );
    okno.draw( zaslona );
    interfejs.rysuj_tekst( okno, L"Poziom ", ::lvl, 300, 220 );
    okno.display();
    sf::sleep( czas_zaslony );
}

void reset( sf::RenderWindow & okno )
{
    lvl = 1 ;
    ilosc_obiektow = poczatkowaIlosc_obiektow;

    ilosc_zabitych = 0;
    potrzeba_zabitych = 5;

    delete [] wsk;
    interfejs.bonusik->x = -500;
    interfejs.bonusik->y = -500;
    interfejs.bonusik->spr.setPosition( -500, -500 );
    ::czy_bonus = 0;
    bonus::sprawdzany = false;
    bonus0.ustaw_timer( 0 );
    ::hp_gracza = 100;
    ::maxHp_gracza = 100;
    inicjalizuj();
    zaslona.setPosition(
        interfejs.kamera.getCenter().x - interfejs.kamera.getSize().x/2,
        interfejs.kamera.getCenter().y - interfejs.kamera.getSize().y/2);
    okno.draw( zaslona );
    interfejs.rysuj_tekst( okno, L"Niestety, tym razem Ci się nie udało ;/", 165, 220 );
//    int nagroda_pocieszenia = 3;
//    if( nagroda_pocieszenia == 3 ){
//        interfejs.rysuj_tekst( okno,
//        L"Tym razem, na początek dostaniesz mały prezent; )", 155, 280 );
////        interfejs.bonusik->x = interfejs.gracz->zwroc_wsp().x;
////        interfejs.bonusik->y = interfejs.gracz->zwroc_wsp().y;
//        interfejs.bonusik->typ = bonus_immune;
//        interfejs.bonusik->sprawdz_bonus();
//    }

    okno.display();
    sf::sleep( czas_zaslony );
    okno.draw( zaslona );
    interfejs.rysuj_tekst( okno, L"Poziom ", ::lvl, 300, 220 );
    okno.display();
    sf::sleep( czas_zaslony );
}

bool aktualizuj()
{
    if( pauza ) return 0;
    for( int i = 0 ; i < ilosc_obiektow - 1 ; i++ )
    {
        wsk[ i ]->aktualizuj_pasekHP();
        wsk[ i ]->aktualizuj_spd();
        wsk[ i ]->kolizja_okno();
        wsk[ i ]->bot_uderz();
        wsk[ i ]->ruch();
        wsk[ i ]->spr.setPosition( wsk[ i ]->zwroc_wsp().x, wsk[ i ]->zwroc_wsp().y );
        wsk[ i ]->aktualizuj_Bothp();
        if( interfejs.bonusik->typ != bonus_immune )
            wsk[ i ]->aktualizuj_timer();
        else
        {
            wsk[ i ]->ustaw_stan( MOZE_ATAKOWAC, false );
            wsk[ i ]->dir = 0;
            wsk[ i ]->spr.setRotation( wsk[ i ]->dir );
        }
        wsk[ i ]->aktualizuj_stanZyje();
    }
//    interfejs.sprawdz_bonus();
    interfejs.bonusik->kolizjaGracz();
    wsk[ ilosc_obiektow - 1 ]->aktualizuj_pasekHP();
    wsk[ ilosc_obiektow - 1 ]->aktualizuj_timer();
    wsk[ ilosc_obiektow - 1 ]->kolizja_okno();
    wsk[ ilosc_obiektow - 1 ]->gracz_uderz();
    wsk[ ilosc_obiektow - 1 ]->steruj();
    wsk[ ilosc_obiektow - 1 ]->spr.setPosition( wsk[ ilosc_obiektow - 1 ]->zwroc_wsp().x,
                             wsk[ ilosc_obiektow - 1 ]->zwroc_wsp().y );
    if ( wsk[ ilosc_obiektow - 1 ]->zwroc_hp() < 0 )
         wsk[ ilosc_obiektow - 1 ]->ustaw_hp( 0 );
    else
    if( wsk[ ilosc_obiektow - 1 ]->zwroc_hp() > wsk[ ilosc_obiektow - 1 ]->zwroc_maxHP() )
        wsk[ ilosc_obiektow - 1 ]->ustaw_hp( wsk[ ilosc_obiektow - 1 ]->zwroc_maxHP() );
    if( wsk[ ilosc_obiektow - 1 ]->zwroc_hp() == 0 )
        wsk[ ilosc_obiektow - 1 ]->ustaw_stan( ZYJE, false );

    wsk[ ilosc_obiektow - 1 ]->ustaw_rozmiarPaskaAtaku( 5 );
    wsk[ ilosc_obiektow - 1 ]->ustaw_pozycjePaskaAtaku(wsk[ ilosc_obiektow - 1 ]->zwroc_wsp().x - 16, wsk[ ilosc_obiektow - 1 ]->zwroc_wsp().y + 17 );
//    interfejs.ustaw_rozmiarPaska( 20 );
//    interfejs.ustaw_pozycjePaska( )

    return 0;
}

void rysuj( sf::RenderWindow & okno )
{
    okno.draw( interfejs.bonusik->spr );
    if( ::ilosc_zabitych < ::potrzeba_zabitych )
    {
        for( int i = 0 ; i < ilosc_obiektow ; i++ )
            if( wsk[ i ]->zwroc_hp() > 0 )
                okno.draw( wsk[ i ]->zwroc_spr() );
        for( int i = 0 ; i < ilosc_obiektow - 1; i++ )
            if( wsk[ i ]->zwroc_hp() > 0 )
                okno.draw( wsk[ i ]->zwroc_pasekHP() );
        okno.draw( wsk[ ilosc_obiektow - 1 ]->zwroc_pasekAtaku() );
    }
    interfejs.rysuj_hud( okno );
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::F1 ) )
        interfejs.credits( okno );
}

int main()
{
    sf::RenderWindow okno( sf::VideoMode( 640, 480 ), "Square War" );
    interfejs.kamera.setSize( 640, 480 );
    okno.setFramerateLimit( 60 );
    srand( time( NULL ) );

    int menu_pozycja = 0;
    int menu_wybor = -1;

    sf::String menu_tresc[ 4 ];
    menu_tresc[ 0 ] = L"Rozpocznij rozgrywkę";
    menu_tresc[ 1 ] = L"Ustawienia";
    menu_tresc[ 2 ] = L"Autorzy";
    menu_tresc[ 3 ] = L"Wyjście";
    sf::Color menu_kolor = sf::Color( 255, 255, 255, 255 );
    bool zmiana_pozycji = true;

    //inicjalizuj();
    obiekt::lista = wsk;
    interfejs.bonusik->bot = obiekt::lista;
    //sf::Music muzyka;
    //muzyka.openFromFile( "sfx/MUZA1.ogg" );
//    muzyka.setLoop( 1 );
//    muzyka.play();

    ::zaslona.setFillColor( sf::Color( 0, 0, 0, 255 ));
    ::zaslona.setSize( sf::Vector2f( 640, 480 ) );
    czas_zaslony = sf::seconds( 3 );


    interfejs.bonusik = &bonus0;
    efektGraficzny tlo;
    int kolor[] = { 0, 0, 255, 255 };
    tex_efektow[ 0 ].loadFromFile( "gfx/swiatlo.png" );
    tex_efektow[ 1 ].loadFromFile( "gfx/swiatlo_bomba.png" );
    tlo.stworzEfekt( tex_efektow[ 1 ],
                     kolor,
                     0,1,1,0.01,0,0,0,0,0,10,0,0,0,0,0 );

    efektGraficzny swiatla[ 8 ];
    int predkoscRotacji = 0;
    for( int i = 0; i < 8; i++ )
    {
        while( predkoscRotacji == 0 )
            predkoscRotacji = losuj( -3, 3 );
        for( int j = 0 ; j < 8 ; j++ )
            kolor[ j ] = losuj( 0, 255 );

        swiatla[ i ].stworzEfekt( ::tex_efektow[ 0 ],
            kolor, 1,
            1, 1,
            0.1, 0, 0,
            losuj( 0, 359 ), predkoscRotacji,
            1, 1, 0, 0, 0,
            55,61
            );
    }

    swiatla[ 0 ].ustaw_koordynaty( 0, 0 );
    swiatla[ 1 ].ustaw_koordynaty( 1024, 0 );
    swiatla[ 2 ].ustaw_koordynaty( 0, 768 );
    swiatla[ 3 ].ustaw_koordynaty( 1024, 768 );
    swiatla[ 4 ].ustaw_koordynaty( 640, 468 );
    swiatla[ 5 ].ustaw_koordynaty( 600, 250 );
    swiatla[ 6 ].ustaw_koordynaty( 264, 200 );
    swiatla[ 7 ].ustaw_koordynaty( 220, 600 );

    while( okno.isOpen() )
    {
        sf::Event event;
        while( okno.pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )
                okno.close();

            if( event.type == sf::Event::KeyPressed ){
                if( event.key.code == sf::Keyboard::Escape )
                    okno.close();
            }
            else
            if( event.type == sf::Event::KeyReleased ){
                if( event.key.code == sf::Keyboard::P )
                    pauza = !pauza;
            }
        }

        while( menu_wybor != MENU_WYJDZ && menu_wybor != MENU_GRAJ )
        {
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) && zmiana_pozycji )
            {
                if( menu_pozycja = 0 ) menu_pozycja = 3;
                else --menu_pozycja;
                zmiana_pozycji = false;
            }
            else
            if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) && zmiana_pozycji )
            {
                if( menu_pozycja = 3 ) menu_pozycja = 0;
                else ++menu_pozycja;
                zmiana_pozycji = false;
            }
            okno.clear( sf::Color::Black );
            for( int i = 0; i < 4 ; i++ ){
                if( i = menu_pozycja )
                    menu_kolor = sf::Color( 255, 0, 0, 255 );
                else menu_kolor = sf::Color( 255, 255, 255, 255 );

                interfejs.rysuj_tekst( okno,  menu_tresc[ i ], 320, 50+20*i );
            }
            okno.display();
            zmiana_pozycji = true;
        }

        aktualizuj();
        if( !pauza ){
            tlo.aktualizujEfekt();
            for( int i = 0; i < 8 ; i++ )
                swiatla[ i ].aktualizujEfekt();
        }

        interfejs.aktualizuj_hud();
        okno.setView( interfejs.kamera );

        if( !pauza ){
            okno.clear();
            tlo.rysuj( okno );
            for( int i = 0; i < 8 ; i++ )
                swiatla[ i ].rysuj( okno );
            rysuj( okno );
        }
        else{
            zaslona.setPosition(
                interfejs.kamera.getCenter().x - interfejs.kamera.getSize().x/2,
                interfejs.kamera.getCenter().y - interfejs.kamera.getSize().y/2);
            okno.draw( zaslona );
            interfejs.rysuj_tekst( okno, "Pauza", 305, 230 );
        }

        if( ::ilosc_zabitych >= ::potrzeba_zabitych )
        {
            nastepny_poziom( okno );
        }

        if( interfejs.zwroc_hpGracza() == 0 )
        {
            ::lvl = 0;
            reset( okno );
        }

        okno.display();
    }
    return 0;
}

/**
KONCEPCJA -:SQUARES WAR:-
-Obiekt
    *Cechy obiektu:
        &Health Point, max i rzeczywista ilosc
        &Wspolrzedne X, Y ( Poprzez klase sf::Vector )
        &Sila ataku
        &Predkosc na 2 zmiennych double
        &Sprite, tekstura pobierana z zewnatrz
        &Pasek zycia
        &Okreslenie stanu obiektu:
            0 - kolizja
            1 - zostal uderzony
            2 - czy jest w ruchu
            3 - czy jest pod wpływem bonusu
            4 - czy moze atakowac
            5 - czy zyje

    *Metody obiektu:
        &Kolizja zwracająca Obiekt - jako prawda / NULL - Fałsz
        &Zmiana Wspolrzednych
        &Zmiana sily ataku
        &Zmiana predkosci
        &Zmiana ilosci hp
        &Kolizja z Oknem
        &Uderz
        &Czy miejsce wolne
        &Metody zwracajace
        &Metoda Aktualizuj
        &Metoda ustaw Teksture
        &Zwroc sprite

        Niewirtualny konstruktor, który na początku w arg ma wartości domyślne
        ( string nazwa = "", double x = 0 , double y = 0, double spd = 0, double hp = 0, double dmg = 0 )
        // Originy automatycznie na ( 16, 16 )

-Gracz
    *Cechy gracza:
        (~Opcjonalnie) Exp i lvl'e
    *Metody gracza:
        &Metoda aktualizuj
        &Metoda ruch
        &
-BOT
    *Cechy bota:
        &Bardzo, bardzo, bardzo, bardzo, bardzo, bardzo, bardzo, bardzo TEMPY

    *Metody bota:
        &Metoda ruch

-Bonusy( TYLKO I WYŁĄCZNIE: Jeden bonus na mapie ):
    *Co bonusy moga dac graczowi:
        +zwiekszenie sily ataku
        +zwiekszenie speed'a
        +odnowienie hp
        +czasowa nietykalnosc

        -spowolnienie
        -odjecie hp

    *Co robi klasa:
        ->Informacja o bonusie jest przesylana do interfejsu
        ->Koncepcja marcina bonus[ ? ]
        ->Metoda nadajaca wylosowany bonus
        ->Wspolrzedne x i y, typ bonusu okresla zmienna int
        ->Wlasny sprite, tekstura wczytywana z zewnatrz

**/
