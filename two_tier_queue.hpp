/*
 MIT License
 Copyright (c) Alex Hagen-Zanker, 2026 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef TWO_TIER_QUEUE_HPP
#define TWO_TIER_QUEUE_HPP

#include <vector>
#include <utility>
#include <stdexcept>


/**
 * @class two_tier_queue
 * @brief A priority queue that holds at most two tiers of priority
 * simultaneously. On pop(), if the current tier is exhausted, the next tier becomes 
 * the current tier.  
 */

template<class T>
class two_tier_queue {
private:
    std::vector<T> current; // Active bucket containing nodes at current tier
    std::vector<T> next;    // Deferred bucket containing nodes at next tier
    int current_tier;         // The current global path cost level

public:
    two_tier_queue(int capacity = 0) : current_tier(0) {
        if (capacity > 0) {
            current.reserve(capacity);
            next.reserve(capacity);
        }
    }

    inline void push_front(const T& val) {
        current.push_back(val);
    }

    inline void push_front(T&& val) {
        current.push_back(std::move(val));
    }

    template<class... Args>
    inline T& emplace_front(Args&&... args) {
        return current.emplace_back(std::forward<Args>(args)...);
    }

    inline void push_back(const T& val) {
        next.push_back(val);
    }

    inline void push_back(T&& val) {
        next.push_back(std::move(val));
    }

    template<class... Args>
    inline T& emplace_back(Args&&... args) {
        return next.emplace_back(std::forward<Args>(args)...);
    }

    inline bool empty() const {
        return current.empty() && next.empty();
    }

    /**
    * @brief Returns the next node with highest priority without removing it.
    * @throws std::out_of_range if queue is empty.
    */
    inline const T& top() const {
       if(!current.empty()) return current.back();
       if(!next.empty()) return next.back();
       throw std::out_of_range("Accessing top of an empty queue.");
    }

    /**
    * @brief Removes the node with the highest priority.
    * @throws std::out_of_range if queue is empty.
    */
    inline void pop() {
       if(current.empty()) {
          if(next.empty()) throw std::out_of_range("Popping an empty queue.");
          std::swap(current, next);
          current_tier++;
         }
       current.pop_back();
    }
    
    inline int tier() const {
        return current_tier;
    }
  
};

#endif // TWO_TIER_QUEUE_HPP
