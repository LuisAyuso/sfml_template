
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include <thread>



int main() {

    sf::Window window(sf::VideoMode(1024, 764), "mywindow");
    window.setVerticalSyncEnabled(true);


    // event loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // end the current frame
        window.display();
    }

    return 0;
}
