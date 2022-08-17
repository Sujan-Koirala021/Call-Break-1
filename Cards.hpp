#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>

class Card{
    private:
        int suit;
        int rank;


    public:
        sf::Sprite sprite;
        static const int eachCardWidth = 140;
        static const int eachCardHeight = 190;
        bool playable=false;
        Card(int s,int r);
        void loadTexture(sf::Texture & texture);
        int getSuit();
        bool isSmlThan(const Card & c1);
        // Card(Card & c1);
        void operator=(const Card & c1);
        void display();
        int getN();
};