#include "GameRenderer.h"

GameRenderer::GameRenderer(Game *game) {
    this->game_ = game;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    main_window_.create(sf::VideoMode(1200, 800), "Teeko", sf::Style::Default, settings);
    sf::Image icon;
    icon.loadFromFile("..\\resources\\images\\icon.png");
    main_window_.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void GameRenderer::Render() {
    sf::Event event;
    while (main_window_.isOpen()) {
        while (main_window_.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {  // Close button
                    main_window_.close();
                    break;
                }
                case sf::Event::Resized: {  // Occur when window is resized
                    main_window_.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    if (event.mouseButton.button == sf::Mouse::Left && game_->GetWinner() == nullptr) {
                        ClickController(sf::Mouse::getPosition(main_window_).x, sf::Mouse::getPosition(main_window_).y);
                    }
                    break;
                }
                case sf::Event::KeyPressed: {
                    if(event.key.code == sf::Keyboard::Escape) {
                        main_window_.close();
                        MainMenu main_menu;
                        main_menu.Render();
                    }
                }
            }
        }

        // TODO Move this to another game loop ; TEST PURPOSES ONLY
        if(game_->IsAIGame() && game_->GetPlayerTurn()==0 && game_->GetPlayers()->at(0).GetSpaces()->size()==4) {
            vector<Space> spaces;
            for(int i = 0; i < game_->GetSpaces()->size(); i++) {
                spaces.push_back(game_->GetSpaces()->at(i));
            }
            vector<int> spaces_id;
            spaces_id = game_->GetAi()->FindBestMoveSpacesId(spaces);
            game_->MoveMarker(game_->GetSpaces()->at(spaces_id.at(0)),game_->GetSpaces()->at(spaces_id.at(1)),0);
        }


        this->DrawBackground();
        this->DrawBoard();
        this->DrawSpaces();
        this->DrawMarkers();
        this->DrawTurnLabel();
        if(game_->GetWinner() != nullptr)
            this->DrawWinnerMessage();

        // Display window
        main_window_.display();
    }
}

void GameRenderer::DrawBoard() {
    sf::RectangleShape border(sf::Vector2f(kBorderX, kBorderY));
    int border_position_x = (main_window_.getSize().x - kBorderX) / 2;
    int border_position_y = (main_window_.getSize().y - kBorderY) / 2;
    border.setPosition(border_position_x, border_position_y);
    border.setFillColor(sf::Color(210, 105, 30));
    border.setOutlineThickness(10);
    border.setOutlineColor(sf::Color(0, 0, 0));
    main_window_.draw(border);

}

void GameRenderer::DrawSpaces() {
    sf::CircleShape spaces[5][5];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            spaces[i][j].setRadius(50);
            spaces[i][j].setFillColor(sf::Color(255, 178, 71));
            spaces[i][j].setOutlineThickness(4);
            spaces[i][j].setOutlineColor(sf::Color::Black);
            // Define spaces position
            int space_position_x = (main_window_.getSize().x - kBorderX) / 2 + i * 150 + 25;
            int space_position_y = (main_window_.getSize().y - kBorderY) / 2 + j * 150 + 25;
            // Set spaces position
            spaces[i][j].setPosition(space_position_x, space_position_y);
            main_window_.draw(spaces[i][j]);
            if (i < 4) { // Render the link between this space and the next one
                sf::RectangleShape link(sf::Vector2f(50, 5));
                link.setPosition(space_position_x + 100, space_position_y + 50);
                link.setFillColor(sf::Color::Black);
                main_window_.draw(link);
            }
            if (j < 4) { // Render the link between this space and the one below it
                sf::RectangleShape link(sf::Vector2f(50, 5));
                link.setFillColor(sf::Color::Black);
                link.setPosition(space_position_x + 50, space_position_y + 100);
                link.rotate(90);
                main_window_.draw(link);
            }
            if (i < 4 && j < 4) { // Render the link between this space and the one on the right diagonal
                int link_position_x = space_position_x + 50 + 35;
                int link_position_y = space_position_y + 50 + 35;
                sf::RectangleShape link(sf::Vector2f(110, 5));
                link.setFillColor(sf::Color::Black);
                link.setPosition(link_position_x, link_position_y);
                link.rotate(45);
                main_window_.draw(link);
            }
            if (i > 0 && j < 4) { // Render the link between this space and the one on the left diagonal
                int link_position_x = space_position_x + 50 - 35;
                int link_position_y = space_position_y + 50 + 35;
                sf::RectangleShape link(sf::Vector2f(110, 5));
                link.setFillColor(sf::Color::Black);
                link.setPosition(link_position_x, link_position_y);
                link.rotate(135);
                main_window_.draw(link);
            }
        }
    }
}

void GameRenderer::DrawMarkers() {
    for (auto &player : *game_->GetPlayers()) {
        vector<Space *> *player_spaces = player.GetSpaces();
        for (int j = 0; j < player_spaces->size(); j++) {
            sf::CircleShape marker(43);
            if (player.GetSpaces()->at(j)->GetMarker()->IsSelected()) {
                marker.setOutlineThickness(-10);
                marker.setOutlineColor(sf::Color(0, 100, 255));
            }
            if (player.GetColor() == "Red")
                marker.setFillColor(sf::Color(216, 0, 0));
            else if (player.GetColor() == "Black")
                marker.setFillColor(sf::Color::Black);
            marker.setPosition(GetCoordX(player_spaces->at(j)->GetSpaceId()) + (50 - marker.getRadius()),
                               GetCoordY(player_spaces->at(j)->GetSpaceId()) + (50 - marker.getRadius()));
            main_window_.draw(marker);
        }
    }
}

void GameRenderer::DrawTurnLabel() {
    sf::Font junegull;
    junegull.loadFromFile("..\\resources\\fonts\\junegull.ttf");

    // Display turn number
    sf::Text turn_text("Turn : " + std::to_string(game_->GetTurnNumber()), junegull);
    turn_text.setCharacterSize(40);
    turn_text.setPosition(20, 10);
    main_window_.draw(turn_text);

    // Display player color
    string player_turn;
    if (game_->GetPlayerTurn() == 0)
        player_turn = "RED";
    else player_turn = "BLACK";
    sf::Text player_turn_text(player_turn, junegull);
    if (game_->GetPlayerTurn() == 0)
        player_turn_text.setFillColor(sf::Color::Red);
    else player_turn_text.setFillColor(sf::Color::Black);
    player_turn_text.setCharacterSize(40);
    player_turn_text.setPosition(20, 50);
    main_window_.draw(player_turn_text);
}

void GameRenderer::DrawBackground() {
    sf::Texture wood_texture;
    if (!wood_texture.loadFromFile("..\\resources\\images\\wood.jpg")) {
        printf("Error while loading wood texture.\n");
    } else {
        wood_texture.setRepeated(true);
        sf::Sprite background_sprite;
        background_sprite.setTexture(wood_texture);
        background_sprite.setTextureRect(sf::IntRect(0, 0, main_window_.getSize().x, main_window_.getSize().y));
        main_window_.draw(background_sprite);
    }
}

int GameRenderer::GetCoordX(int space_id) {
    int x = space_id % 5;
    if (x == 0) {
        x = 4;
    } else
        x--;
    return (main_window_.getSize().x - 750) / 2 + x * 150 + 25;
}

int GameRenderer::GetCoordY(int space_id) {
    int y = space_id / 5;
    if (space_id % 5 == 0) {
        y = space_id / 5 - 1;
    }
    return (main_window_.getSize().y - 750) / 2 + y * 150 + 25;
}

// TODO(Piryus) Refactor this method
void GameRenderer::ClickController(int mouse_x, int mouse_y) {
    vector<Space> *spaces = game_->GetSpaces();
    int turn_number = game_->GetTurnNumber();
    int player_turn = game_->GetPlayerTurn();
    Space *selected_space = nullptr;
    int clicked_space_id = GetClickedSpaceID(mouse_x, mouse_y);

    for (int j = 0; j < spaces->size(); j++) {
        if (spaces->at(j).GetMarker() != nullptr && spaces->at(j).GetMarker()->IsSelected())
            selected_space = &spaces->at(j);
    }

    if (selected_space == nullptr &&
        clicked_space_id != -1) { // If the player clicks on a space and if no space is selected
        // If the player clicks on a marker
        if (spaces->at(clicked_space_id - 1).GetMarker() != nullptr &&
            (spaces->at(clicked_space_id - 1).GetMarker()->GetColor() ==
                    game_->GetPlayers()->at(game_->GetPlayerTurn()).GetColor()))
            spaces->at(clicked_space_id - 1).GetMarker()->Select();
            // If he clicks on an empty space
        else game_->PlaceMarker(spaces->at(clicked_space_id - 1), player_turn);
    } else if (selected_space != nullptr &&
               clicked_space_id == -1) { // If the player clicks out of a space and if a space is selected
        // Unselect the space
        selected_space->GetMarker()->Unselect();
        selected_space = nullptr;
    } else if (selected_space != nullptr &&
               clicked_space_id != -1) { // If the player clicks on a valid space and if there is a selected marker
        // If the player clicked on a marker
        if (spaces->at(clicked_space_id - 1).GetMarker() != nullptr &&
            (spaces->at(clicked_space_id - 1).GetMarker()->GetColor() ==
                    game_->GetPlayers()->at(game_->GetPlayerTurn()).GetColor())) {
            // Unselect the selected space
            selected_space->GetMarker()->Unselect();
            selected_space = nullptr;
            // Select the other marker
            spaces->at(clicked_space_id - 1).GetMarker()->Select();
        } else {
            selected_space->GetMarker()->Unselect();
            game_->MoveMarker(spaces->at(selected_space->GetSpaceId() - 1), spaces->at(clicked_space_id - 1),
                              player_turn);
        }
    }
}

int GameRenderer::GetClickedSpaceID(int x, int y) {
    int space_id = -1;
    for (int i = 0; i < 25; i++) {
        if ((x >= GetCoordX(i + 1) && x <= GetCoordX(i + 1) + 100) &&
            (y >= GetCoordY(i + 1) && y <= GetCoordY(i + 1) + 100)) {
            space_id = i + 1;
        }
    }
    return space_id;
}

void GameRenderer::DrawWinnerMessage() {
    // Hide the game with black mask
    sf::RectangleShape black_mask(sf::Vector2f(main_window_.getSize().x,main_window_.getSize().y));
    black_mask.setFillColor(sf::Color(0,0,0,150));
    main_window_.draw(black_mask);

    // Load font
    sf::Font junegull;
    junegull.loadFromFile("..\\resources\\fonts\\junegull.ttf");

    // Display winner
    sf::Text winner(game_->GetWinner()->GetColor() + " wins!", junegull);
    winner.setCharacterSize(70);
    winner.setOrigin(winner.getGlobalBounds().width / 2, winner.getGlobalBounds().height / 2);
    winner.setPosition(main_window_.getSize().x / 2,
                       main_window_.getSize().y / 2);
    main_window_.draw(winner);
}
