#ifndef FINDER_WORLD_H
#define FINDER_WORLD_H

#include <vector>
#include <chrono>

#include <boost/thread/thread.hpp>

#include <queue>
#include <boost/date_time.hpp>
#include <ObjectManager.h>
#include <PacketManagerServer.h>
#include <NetServer.h>

const static int SECONDS_PER_MINUTE = 60;
const static double FRAMES_PER_SECOND = 15;

class World {
public:
    World(int player_count, int game_duration_minute, int port, int lay_count, double r_radius):
          netServer(port), player_count(player_count), layers_count(lay_count), ring_radius(r_radius){
        game_duration = game_duration_minute * SECONDS_PER_MINUTE;
        tick_duration = 1 / FRAMES_PER_SECOND;
        map_centre = Point(WINDOW_W / 2, WINDOW_H / 2);
    }
    void game_start();

private:

    ObjectManager objectManager;
    std::queue<std::shared_ptr<Event>> queque_event;
    NetServer netServer;
    Point map_centre;
    double tick_duration;
    double game_duration;
    int player_count;
    int layers_count;
    double ring_radius;

    void calc_frame();
    std::shared_ptr<Player> init_user(User& user);
    void set_start_object();
    void serve_user(User& user);
    std::mutex events_m;
    bool need_update;
    bool game_is_go_on;
};

void World::calc_frame() {
    while(game_is_go_on) {
        std::unordered_map<int, std::shared_ptr<Object>>& objects = objectManager.get_objects_by_map();
        if (need_update) {
            for (auto& object: objects) {
                if (object.second->type != Object::STATIC_OBJECT) {
                    object.second->update();
                    auto collisions = objectManager.collisionSolver.check_object_collisions(objects, object.second);
                    for (auto& collision: collisions) {
                        objectManager.collisionSolver.resolve_collision(object.second, collision);
                    }
                }
            }
            need_update = false;
        } else {
            std::lock_guard<std::mutex> lock(events_m);
            if (!queque_event.empty()) {
                std::shared_ptr<Event> event = queque_event.front();

                queque_event.pop();
                auto object = objectManager.get_object_by_id(event->IniciatorID);
                //Object new_state_object = event.get()->proccess(object); получаем новое стстояние обекиа
                auto New_state = event->proccess(object, objectManager);
                if (!objectManager.collisionSolver.is_object_collided(objects, New_state)) { //проверяем есть ли коллиизиb
                    //с новым состоянием
                    *object = *New_state;
                    // objectManager.update_objects(New_state);
                }
            }
            //обрабатываем ивент из очереди
        }
    }
};// При наличии флага обнавления вызывает update у всех обьектов, иначе исполняет Event

void World::game_start() {
    std::vector<User> players_init = netServer.accept_users(player_count, objectManager);
    std::vector<boost::thread> threads;
    for (auto& usr: players_init) {
        objectManager.update_objects(init_user(usr));
        boost::thread  th([&](){
            this->serve_user(usr);
        });
        threads.push_back(move(th));
    }
    set_start_object();
    game_is_go_on = true;
    boost::thread  th([&](){
        this->calc_frame();
    });
    threads.push_back(move(th));
    auto round_start = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration current_game_duration;
    boost::posix_time::time_duration current_tick_duration;
    auto last_tick = boost::posix_time::microsec_clock::universal_time();
    while (current_game_duration.total_seconds() < game_duration) {
        auto curr_time = boost::posix_time::microsec_clock::universal_time();
        current_tick_duration = curr_time - last_tick;
        if ((current_tick_duration.total_milliseconds() / 1000.0) > tick_duration) {
            last_tick = curr_time;
            need_update = true;
            netServer.notify_all_users(objectManager.get_objects_by_map());
        }
        curr_time = boost::posix_time::microsec_clock::universal_time();
        current_game_duration = curr_time - round_start;
    }
    game_is_go_on = false;
    for (auto& th: threads) {
        th.join();
    }
}

void World::serve_user(User& user) {
    while(game_is_go_on) {
        std::shared_ptr<Event> event = netServer.get_client_action(user);
        std::lock_guard<std::mutex> lock(events_m);
        queque_event.push(event);
    }
}

std::shared_ptr<Player> World::init_user(User &user) {
    int id = user.get_username();
    Point position(map_centre.x + (pow(-1, id) * 300), map_centre.y + (pow(-1, player_count - id) * 300));
    std::shared_ptr<Player> player = std::make_shared<Player>(id, position);
    return player;
}

void World::set_start_object() {
    std::vector<std::shared_ptr<Object>> players;
    for (int i = 0; i < player_count; i++) {
        players.push_back(objectManager.get_object_by_id(i));
    }
    std::shared_ptr<Map> map = std::make_shared<Map>(objectManager.pick_enable_id(), layers_count, ring_radius,
                                                     game_duration * FRAMES_PER_SECOND, move(players));
    objectManager.update_objects(map);

    std::shared_ptr<Obstruction> obs1 = std::make_shared<Obstruction>(objectManager.pick_enable_id(), Point(460, 280), 30, 250);
    objectManager.update_objects(obs1);

    std::shared_ptr<Obstruction> obs2 = std::make_shared<Obstruction>(objectManager.pick_enable_id(), Point(520, 200), 250, 30);
    objectManager.update_objects(obs2);

    std::shared_ptr<Obstruction> obs3 = std::make_shared<Obstruction>(objectManager.pick_enable_id(), Point(800, 280), 30, 250);
    objectManager.update_objects(obs3);

    std::shared_ptr<Obstruction> obs4 = std::make_shared<Obstruction>(objectManager.pick_enable_id(), Point(520, 560), 250, 30);
    objectManager.update_objects(obs4);
}

#endif //FINDER_WORLD_H