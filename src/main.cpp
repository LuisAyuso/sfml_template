
#include "utils/channel.h"


#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <thread>

// needed for XInintThreads, and do rendering in a second thread
#include <X11/Xlib.h>



void render_thread(sf::Window *window, myapp::receiver<sf::Event> queue) {
    window->setActive();

    bool running = true;
    while (running) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // end the current frame
        window->display();

        auto event = queue.recv();

        switch (event.type) {
        case sf::Event::Closed:
            running = false;
            break;
        default:
            break;
        }
    }
}



int main() {

    // without this rendering in second thread crashes
    XInitThreads();

    sf::Window window(sf::VideoMode(1024, 764), "mywindow");
    window.setVerticalSyncEnabled(true);
    window.setActive(false);


    auto[send, receive] = myapp::make_producer_consumer_pair<sf::Event>();

    std::thread th(render_thread, &window, std::move(receive));

    // event loop
    bool running = true;
    while (running) {
        sf::Event event;
        while (window.pollEvent(event)) {

            send.send(sf::Event(event));

            switch (event.type) {
            case sf::Event::Closed:
                th.join();
                running = false;
                break;
            default:
                break;
            }
        }
    }

    return 0;
}
