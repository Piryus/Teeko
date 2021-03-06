#include <iostream>
#include "MainMenu.h"

/**
 * Default constructor
 * The main menu is the first thing appearing at the screen, it has to create a window to be displayed in
 */
MainMenu::MainMenu() {
    // Create window rendering settings
    sf::ContextSettings settings;
    // Sets the antialiasing level to 8 to smooth nicely the shapes, buttons, ...
    settings.antialiasingLevel = 8;
    // Creates a new window in full screen mode with the previously defined settings
    main_window_->create(sf::VideoMode::getFullscreenModes()[0], "Teeko", sf::Style::Fullscreen, settings);
    // Sets the window's icon
    sf::Image icon;
    icon.loadFromFile(R"(..\resources\images\icon.png)");
    main_window_->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Creates the different buttons of the menu
    // Buttons position, first button is at the center of the window
    int button_x = (main_window_->getSize().x - kButtonWidth) / 2;
    int button_y = (main_window_->getSize().y - kButtonHeight) / 2;

    // Creates play button
    play_two_button_ = CreateButton(button_x, button_y, sf::Color(230, 126, 34), sf::Color(211, 84, 0));

    // Creates "Play against AI" button, 50px under the previous one
    button_y += kButtonHeight + 50;
    play_ai_button_ = CreateButton(button_x, button_y, sf::Color(230, 126, 34), sf::Color(211, 84, 0));

    // Creates "Play against AI" button, 50px under the previous one
    button_y += kButtonHeight + 50;
    spectator_button_ = CreateButton(button_x, button_y, sf::Color(230, 126, 34), sf::Color(211, 84, 0));

    // Create "Exit" button, 50px under the previous one
    button_y += kButtonHeight + 50;
    exit_button_ = CreateButton(button_x, button_y, sf::Color(230, 126, 34), sf::Color(211, 84, 0));
}

/**
 * Renders the main menu
 * Contains the main loop of the menu and will call appropriate functions on click
 */
void MainMenu::Render() {
    sf::Event event{};
    while (main_window_->isOpen()) {
        while (main_window_->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {  // Close button
                    main_window_->close();
                    break;
                }
                case sf::Event::Resized: {  // Occurs when window is resized
                    main_window_->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                    // Buttons need to be moved
                    // Buttons position, first button is at the center of the window
                    int button_x = (main_window_->getSize().x - kButtonWidth) / 2;
                    int button_y = (main_window_->getSize().y - kButtonHeight) / 2;
                    play_two_button_.setPosition(button_x, button_y);
                    button_y += kButtonHeight + 50;
                    play_ai_button_.setPosition(button_x, button_y);
                    button_y += kButtonHeight + 50;
                    spectator_button_.setPosition(button_x, button_y);
                    button_y += kButtonHeight + 50;
                    exit_button_.setPosition(button_x, button_y);
                    break;
                }
                case sf::Event::MouseButtonReleased: { // On player's click
                    ClickController(sf::Mouse::getPosition(*main_window_).x,
                                    sf::Mouse::getPosition(*main_window_).y);
                    break;
                }
            }
        }

        // Draws the menu's elements
        this->DrawBackground();
        this->DrawTitle();
        this->DrawButtons();
        // Displays the window
        main_window_->display();
    }
}

/**
 * Loads the "Teeko" logo image and displays it at the top of the menu
 */
void MainMenu::DrawTitle() {
    // Loads the logo
    sf::Texture teeko_img;
    teeko_img.loadFromFile(R"(..\resources\images\teeko.png)");
    // Creates a sprite fitting the logo
    sf::Sprite teeko;
    teeko.setTexture(teeko_img);
    // The logo's position depends of the width and height of the window
    teeko.setPosition((main_window_->getSize().x - teeko.getLocalBounds().width) / 2,
                      (main_window_->getSize().y - teeko.getLocalBounds().height) / 12);
    main_window_->draw(teeko);
}

/**
 * Displays the menu buttons and their texts
 */
void MainMenu::DrawButtons() {
    // If mouse is on a button, change its color
    ButtonsColorController(sf::Mouse::getPosition(*main_window_).x,
                           sf::Mouse::getPosition(*main_window_).y);

    // Draw the buttons
    main_window_->draw(play_two_button_);
    main_window_->draw(play_ai_button_);
    main_window_->draw(spectator_button_);
    main_window_->draw(exit_button_);

    // Draw button texts
    main_window_->draw(CreateButtonText(&play_two_button_, "Play (2 players)"));
    main_window_->draw(CreateButtonText(&play_ai_button_, "Play against A.I."));
    main_window_->draw(CreateButtonText(&spectator_button_, "Spectator mode"));
    main_window_->draw(CreateButtonText(&exit_button_, "Exit game"));
}

/**
 * Checks whether the mouse pointer is on a button
 * If it is, sets the fill color of the button to green and plays a click sound
 * If not, sets the color to orange
 * @param mouse_x, the mouse position on X axis
 * @param mouse_y, the mouse position on Y axis
 */
void MainMenu::ButtonsColorController(int mouse_x, int mouse_y) {
    // Creates a vector with the menu's buttons
    std::vector<sf::RectangleShape*> buttons_vec{&play_two_button_, &play_ai_button_, &spectator_button_, &exit_button_};
    for (sf::RectangleShape *button : buttons_vec) {
        // Checks if mouse pointer is on the button
        if (mouse_x >= button->getPosition().x &&
            mouse_x <= (button->getPosition().x + button->getSize().x) &&
            mouse_y >= button->getPosition().y &&
            mouse_y <= (button->getPosition().y + button->getSize().y)) {
            // Changes its color to green and plays a click sound
            if(button->getFillColor() != sf::Color(39, 174, 96)) {
                button->setFillColor(sf::Color(39, 174, 96));
                sound_manager_.PlayButtonClickSound();
            }
        } else button->setFillColor(sf::Color(230, 126, 34)); // If the cursor isn't on the button, sets its color to normal
    }
}

/**
 * Manages events on click
 * Checks whether the player clicked on a button or not
 * If he did, executes the action linked to that button
 * @param mouse_x, the mouse position on the X axis on click
 * @param mouse_y, the mouse position on the Y axis on click
 */
void MainMenu::ClickController(int mouse_x, int mouse_y) {
    // Creates a vector with the menu's buttons
    std::vector<sf::RectangleShape> buttons_vec{play_two_button_, play_ai_button_, spectator_button_,  exit_button_};
    // Here we use the int i to iterate through the vector because buttons don't have IDs and we need to identify which button was clicked
    for (int i = 0; i < buttons_vec.size(); i++) {
        if (mouse_x >= buttons_vec.at(i).getPosition().x &&
            mouse_x <= (buttons_vec.at(i).getPosition().x + buttons_vec.at(i).getSize().x) &&
            mouse_y >= buttons_vec.at(i).getPosition().y &&
            mouse_y <= (buttons_vec.at(i).getPosition().y + buttons_vec.at(i).getSize().y)) {
            switch (i) {
                case 0 : { // Two players button
                    Game game(false, -1, false);
                    GameRenderer game_renderer(&game, main_window_);
                    game_renderer.Render();
                    break;
                }
                case 1 : { // AI button
                    AIMenu ai_menu(main_window_);
                    ai_menu.Render();
                    break;
                }
                case 2 : { // Spectator mode button
                    SpectatorMenu spectator_menu(main_window_);
                    spectator_menu.Render();
                    break;
                }
                case 3 : { // Exit
                    main_window_->close();
                    break;
                }
                default:
                    break;
            }
        }
    }
}
