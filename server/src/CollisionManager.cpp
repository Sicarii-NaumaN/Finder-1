#include <CollisionManager.h>
#include <unordered_map>
#include <include/CollisionManager.h>


std::vector<std::shared_ptr<Object>>
CollisionManager::check_object_collisions(const std::unordered_map<int, std::shared_ptr<Object>> &objects,
                                          std::shared_ptr<Object> object) {
    std::vector<std::shared_ptr<Object>> result;
    for (auto& obj: objects) {
        if (is_collided(obj.second, object)) {
            if (obj.second->ID != object->ID) {
                result.push_back(obj.second);
            }
        }
    }
    return result;
}
