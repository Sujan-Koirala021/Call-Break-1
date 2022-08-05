#include "Play.hpp"
#include "Menu.hpp"
#include <bits/stdc++.h>

// Used for sorting Cards
bool compare(Card & c1, Card & c2){
    return c1.getN() > c2.getN();
}

void Play::playGame(){ 

    //  All the rough game code remains here

    std::vector<Card> allCards; //It is a vector for all 52 cards
    std::vector<Card> card1;
    std::vector<Card>::iterator f;

    // Initializing all the cards and adding to allCards(which is an array or collection)
    for(int i =0;i<4;i++){
        for(int j = 0;j<13;j++){
            allCards.push_back(Card(i,j));
        }
    }
    // shuffling cards (doesn't work) currently
    std::random_shuffle(allCards.begin(),allCards.end());
    
    
    sf::Texture cardFrontTexture;
	if (!cardFrontTexture.loadFromFile("src\\Images\\Cards\\cardFront\\all_cards.png"))
	{
		std::cout << "Error loading card image.";
	}

    //  Load texture for card back
	if (!cardBackTexture.loadFromFile("src\\Images\\Cards\\cardBack\\cardBack_red1.png"))
	{
		std::cout << "Error loading cardBack image.";
	}

    //  Array with 3 elements containing cardBack at 3 positions

    showCardBacks();

    // seperate first 13 cards
    card1.assign(allCards.begin(),allCards.begin()+13);
    sort(card1.begin(),card1.end(),compare);
    loadPlayerCard(card1, cardFrontTexture);
    
    //load texture for the 13 cards
    for(auto i=card1.begin();i!=card1.end();++i){
            i->loadTexture(cardFrontTexture);
        }
    
    //  Texture for table
    sf::Texture table_texture;
    if (!table_texture.loadFromFile("src/Images/table_back.jpg"))
    {
        std::cout << "Error loading table background image.";
    }
    sf::Sprite tableBackground(table_texture);


    //create a player using those cards
    Player p1(card1, true);

    //display players first card
    sf::RenderWindow GAME_WINDOW;
    GAME_WINDOW.create(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Play Call Break");
    while (GAME_WINDOW.isOpen())
    {
		sf::Event event;
		while (GAME_WINDOW.pollEvent(event))
		{
				if (event.type == sf::Event::Closed)
				{
					GAME_WINDOW.close();
				}
                if (event.type == sf::Event::KeyPressed)
					{
						//	Esc  key pressed
						if (event.key.code == sf::Keyboard::Escape)
                        {
							//	Escape key pressed closes credits window
							GAME_WINDOW.close();

                            Menu menu;
                            menu.openMainMenuWindow();

						}
                    }
        }

        GAME_WINDOW.clear();
        GAME_WINDOW.draw(tableBackground);

        displayPlayerCards(GAME_WINDOW, p1);
        // GAME_WINDOW.draw(p1.cards[0].sprite); 
        GAME_WINDOW.draw(cardBack[0]);
        GAME_WINDOW.draw(cardBack[1]);
        GAME_WINDOW.draw(cardBack[2]);

        //  Check for click of all 13 player cards
        for (int i = 0; i<13; i++){
            checkToMoveCardForward(p1.cards[i].sprite, GAME_WINDOW);
        }

        GAME_WINDOW.display();   
        if (p1.bidWindowShown){
            showBidWindow(p1.bidWindowShown);
        } 

    }

}

//  Function to return sprite for back of card sprite

sf::Sprite Play::getCardBackSprite(sf::Texture & texture, int X_POS, int Y_POS, bool rotateFlag){
    sf::Sprite backSprite(texture);

    //  Rotate sprite if flag set to true
    if (rotateFlag){
        backSprite.setRotation(90);
    }
    //  Specify position to blit on screen in vector form
    backSprite.setPosition(sf::Vector2f(X_POS , Y_POS));

    backSprite.setTextureRect(sf::IntRect(0, 0,140,190)); 

    //  Scale card back img    
    backSprite.scale(sf::Vector2f(0.6, 0.6));
    return backSprite;
}

void Play::showCardBacks()
{
        //  Get Sprite with specified arguments
    cardBack[0] = getCardBackSprite(cardBackTexture, GAME_WIDTH/2 - Card::eachCardWidth/2, 0, false);
    cardBack[1] = getCardBackSprite(cardBackTexture, 130, GAME_HEIGHT/2 - Card::eachCardHeight/2, true);
    cardBack[2] = getCardBackSprite(cardBackTexture, GAME_WIDTH, GAME_HEIGHT/2 - Card::eachCardHeight/2, true);

}

void Play::loadPlayerCard(std::vector<Card>& card, sf::Texture& texture)
{
    //load texture for the 13 cards
    for(auto i=card.begin();i!=card.end();++i){
            i->loadTexture(texture);
        }
}

//  Show player cards
void Play::displayPlayerCards(sf::RenderWindow &window, Player& p_1){
    int size = p_1.cards.size();
    for (int i=0; i < size; i++)
    {
        sf::Sprite &p_sprite= p_1.cards[i].sprite; 
        //  define postion for player cards
        p_sprite.setPosition(sf::Vector2f(GAME_WIDTH/2-((size-1)*55+112)/2 + 55*(i), GAME_HEIGHT-150));
        window.draw(p_sprite); 
    }

}

//returns the index of winning card
int Play::getWinner(std::vector<Card> & gameCards){
    int maxInd = 0;

    for(auto i = gameCards.begin();i<gameCards.end();++i ){
        if (gameCards[maxInd].isSmlThan(*i)){
            maxInd = i - gameCards.begin();
        }
    }
    return maxInd;
}


//sets playable member of legal cards to true and other cards to false
//legal cards are those cards which a player is allowed to throw given the gameCards
//gameCards is a vector of Cards that prior players have thrown
void Play::selectLegalCards (std::vector<Card> & playerCards, std::vector<Card> & gameCards){
    if (gameCards.size() == 0){
        for(auto i=playerCards.begin();i<playerCards.end();++i){
            i->playable = true;
        }
    }
    else {
        Card maxCard = gameCards[0];
        int gameSuit = gameCards[0].getSuit();
        bool setFlag = false;
        bool containSpade = false;
        for(auto i = gameCards.begin();i<gameCards.end();++i ){
            if (i->getSuit()==3 && gameSuit!=3){
                containSpade = true;
            }
            if (maxCard.isSmlThan(*i) && (i->getSuit()!=3|| gameSuit==3) ){
                maxCard = *i;
            }

        }

        for(auto i=playerCards.begin();i<playerCards.end();++i){
            if(maxCard.isSmlThan(*i) && (i->getSuit()==gameSuit)){
                i->playable = true;
                setFlag = true;
            }
            else{
                i->playable = false;
            }
        }

        if (!setFlag || containSpade){
            for(auto i=playerCards.begin();i<playerCards.end();++i){
                if(i->getSuit()==gameSuit){
                    i->playable = true;
                    setFlag = true;
                }
            }
        }

        if (!setFlag && gameSuit!=3){
            for(auto i=playerCards.begin();i<playerCards.end();++i){
                if(i->getSuit()==3){
                    i->playable = true;
                    setFlag = true;
                }
            }
        }

        if (!setFlag){
            for(auto i=playerCards.begin();i<playerCards.end();++i){
                i->playable = true; 
                setFlag = true;
            }  
        }
    }
}

//  Check if sprite is clicked through mouse
bool Play::checkForMouseTrigger(sf::Sprite &av_Sprite, sf::RenderWindow &av_Window)
{

    int mouseX = sf::Mouse::getPosition().x;
    int mouseY = sf::Mouse::getPosition().y;
    sf::Vector2i windowPosition = av_Window.getPosition();

    if(mouseX > av_Sprite.getPosition().x + windowPosition.x && mouseX < ( av_Sprite.getPosition().x + av_Sprite.getGlobalBounds().width + windowPosition.x - 20)
        && mouseY > av_Sprite.getPosition().y + windowPosition.y + 30  && mouseY < ( av_Sprite.getPosition().y + av_Sprite.getGlobalBounds().height + windowPosition.y + 30) )
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            return true;
        }
        return false;
    }
    return false;
}

//  Small Bid window
void Play::showBidWindow(bool &bidWinShown){
    sf::RenderWindow BID_WINDOW;

    BID_WINDOW.create(sf::VideoMode(430, 210), "Call Your Bid", sf::Style::None);

    	//	Load texture for background image and load img
	sf::Texture texture;
	if (!texture.loadFromFile("src/Images/bidWindowBackground.jpg"))
	{
		std::cout << "Error loading bid window background image.";
	}
	sf::Sprite bidWinBackground(texture);
	bidWinBackground.scale(sf::Vector2f(0.8, 0.8));


    while (BID_WINDOW.isOpen())
    {
		sf::Event event;
		while (BID_WINDOW.pollEvent(event))
		{
				if (event.type == sf::Event::Closed)
				{
					BID_WINDOW.close();
				}
                if (event.type == sf::Event::TextEntered)
                {
                    int num = event.text.unicode - 48;  // get ascii for digit entered and convert again to num
                    //  To open window just once in game loop
                    bidWinShown = false;
                    if (num>0 && num<=8){
                        std::cout << "Bid entered = " << num;   //  This is the user input bid no
                        //  Close bid window only for bid range 1 - 8
                        BID_WINDOW.close();
                    }
                }
        }
        BID_WINDOW.clear();
        BID_WINDOW.draw(bidWinBackground);
        BID_WINDOW.display();
    }
}

//  Function to move sprite (but doesn't at the moment )
void Play::checkToMoveCardForward(sf::Sprite &card_sprite, sf::RenderWindow &window){
        bool mouseClick = checkForMouseTrigger(card_sprite, window);
        if(mouseClick)
        {
            std::cout << "Card clicked" <<std::endl;
            // card_sprite.move(sf::Vector2f(400, 400));
        }
}