#include "../include/utils/broadcaster.hpp"

int broadcast_val(std::vector<int> shape1, std::vector<int> shape2) {
    while (shape1.size() < shape2.size()) {
        shape1.insert(shape1.begin(), 1);
    }

    while (shape2.size() < shape1.size()) {
        shape2.insert(shape2.begin(), 1);
    }

    int broadcast_val = -1;
    bool changed = false;
    // only supporting one broadcasting shape?
    for (int i = 0; i < shape2.size(); i++) {
        if (shape2[i] == shape1[i] && shape1[i] != 1) {
            broadcast_val = i;
        } else if (shape2[i] != shape1[i]) {
            changed = true;
        }
    }

    return changed ? broadcast_val : 0;
}