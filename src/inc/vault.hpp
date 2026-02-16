// -------------------------------------------------------------
//
// !\file vault.hpp
// !\brief Vault class for managing a dynamic array of objects
// !\details Owns objects and tracks logical slot pointers (owned or external)
// !\author Colin J.D. Stewart
//
// -------------------------------------------------------------
//
//             Copyright (c) 2025. Colin J.D. Stewart
//                     All rights reserved
//
// -------------------------------------------------------------

#ifndef VAULT_HPP
#define VAULT_HPP

// system
#include <cstdlib>
#include <new>
#include <stdexcept>
#include <utility>

template<typename T, size_t MaxCheckpoints = 64>
class vault {
private:
    T* buffer = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;

    T** m_slots = nullptr;
    size_t m_stack = 0;
    size_t m_slots_cap = 0;

    size_t checkpoint_stack[MaxCheckpoints];
    size_t checkpoint_count = 0;

    bool sealed = false;
    bool locked = false;

    void grow_buffer() {
        size_t newCap = (m_capacity ? m_capacity * 2 : 16);
        T* newBuf = static_cast<T*>(std::malloc(newCap * sizeof(T)));
        if (!newBuf) throw std::bad_alloc();

        for (size_t i = 0; i < m_size; ++i) {
            new (newBuf + i) T(std::move(buffer[i]));
            buffer[i].~T();
        }

        std::free(buffer);
        buffer = newBuf;
        m_capacity = newCap;
    }

    void grow_slots() {
        size_t newCap = (m_slots_cap ? m_slots_cap * 2 : 32);
        T** newSlots = static_cast<T**>(std::realloc(m_slots, newCap * sizeof(T*)));
        if (!newSlots) throw std::bad_alloc();
        m_slots = newSlots;
        m_slots_cap = newCap;
    }

public:
    vault() = default;

    ~vault() {
        clear();
        std::free(buffer);
        std::free(m_slots);
    }

    vault(const vault&) = delete;
    vault& operator=(const vault&) = delete;

    vault(vault&& other) noexcept {
        buffer = other.buffer;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        m_slots = other.m_slots;
        m_stack = other.m_stack;
        m_slots_cap = other.m_slots_cap;

        sealed = other.sealed;
        locked = other.locked;
        checkpoint_count = other.checkpoint_count;
        for (size_t i = 0; i < checkpoint_count; ++i)
            checkpoint_stack[i] = other.checkpoint_stack[i];

        other.buffer = nullptr;
        other.m_slots = nullptr;
        other.m_size = 0;
        other.m_stack = 0;
        other.m_capacity = 0;
        other.m_slots_cap = 0;
        other.checkpoint_count = 0;
    }

    vault& operator=(vault&& other) noexcept {
        if (this != &other) {
            clear();
            std::free(buffer);
            std::free(m_slots);

            buffer = other.buffer;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            m_slots = other.m_slots;
            m_stack = other.m_stack;
            m_slots_cap = other.m_slots_cap;

            sealed = other.sealed;
            locked = other.locked;
            checkpoint_count = other.checkpoint_count;
            for (size_t i = 0; i < checkpoint_count; ++i)
                checkpoint_stack[i] = other.checkpoint_stack[i];

            other.buffer = nullptr;
            other.m_slots = nullptr;
            other.m_size = 0;
            other.m_stack = 0;
            other.m_capacity = 0;
            other.m_slots_cap = 0;
            other.checkpoint_count = 0;
        }
        return *this;
    }

    void reserve(size_t newCap) {
        if (sealed) throw std::runtime_error("vault is sealed; cannot reserve.");
        if (newCap <= m_capacity) return;

        T* newBuf = static_cast<T*>(std::malloc(newCap * sizeof(T)));
        if (!newBuf) throw std::bad_alloc();

        for (size_t i = 0; i < m_size; ++i) {
            new (newBuf + i) T(std::move(buffer[i]));
            buffer[i].~T();
        }

        std::free(buffer);
        buffer = newBuf;
        m_capacity = newCap;
    }

    template<typename... Args>
    T* emplace(Args&&... args) {
        if (sealed) throw std::runtime_error("vault is sealed; cannot emplace.");
        if (m_stack >= m_slots_cap) grow_slots();
    
        T* ptr;
    
        if (m_stack < m_size) {
            // Reuse an existing owned object slot
            ptr = &buffer[m_stack];
            // Destroy previous value before placement-new
            ptr->~T();
            new (ptr) T(std::forward<Args>(args)...);
        } else {
            if (m_size >= m_capacity) grow_buffer();
            ptr = &buffer[m_size];
            new (ptr) T(std::forward<Args>(args)...);
            ++m_size;
        }
    
        m_slots[m_stack++] = ptr;
        return ptr;
    }
    

    T* acquire() {
        if (sealed) throw std::runtime_error("vault is sealed; cannot acquire.");
        if (m_stack >= m_slots_cap) grow_slots();
    
        T* ptr;
    
        if (m_stack < m_size) {
            ptr = &buffer[m_stack];  // safe reuse
        } else {
            if (m_size >= m_capacity) grow_buffer();
            ptr = &buffer[m_size];
            new (ptr) T();  // construct default object!
            ++m_size;
        }
    
        m_slots[m_stack++] = ptr;
        return ptr;
    }
    


    void checkin(T* external) {
        if (!external) throw std::runtime_error("vault::checkin null pointer");
        if (m_stack >= m_slots_cap) grow_slots();
        m_slots[m_stack++] = external;
    }

    T* get(size_t index) {
        if (index >= m_stack) throw std::out_of_range("vault::get out of range");
        return m_slots[index];
    }

    const T* get(size_t index) const {
        if (index >= m_stack) throw std::out_of_range("vault::get const out of range");
        return m_slots[index];
    }

    T& operator[](size_t index) { T* p = get(index); return *p; }
    const T& operator[](size_t index) const { const T* p = get(index); return *p; }

    T* top() {
        if (m_stack == 0) throw std::runtime_error("vault::top empty");
        return m_slots[m_stack - 1];
    }

    const T* top() const {
        if (m_stack == 0) throw std::runtime_error("vault::top empty");
        return m_slots[m_stack - 1];
    }

    void pop() {
        if (m_stack == 0) throw std::runtime_error("vault::pop underflow");
        --m_stack;
    }

    size_t size() const { return m_stack; }           // Stack size
    bool empty() const { return m_stack == 0; }
    size_t count() const { return m_stack; }           // Number of total slot pointers (owned + external)
    size_t capacity() const { return m_capacity; }    // Capacity for owned elements
    size_t max_size() const { return m_capacity; }    // Alias for capacity()
    size_t pointer_count() const { return m_stack; }  // Number of total slot pointers (owned + external)
    size_t owned_count() const { return m_size; }     // Number of owned elements


    void clear_stack() { m_stack = 0; }

    void checkpoint() {
        if (checkpoint_count >= MaxCheckpoints)
            throw std::runtime_error("vault checkpoint overflow");
        checkpoint_stack[checkpoint_count++] = m_stack;
    }

    void restore() {
        if (checkpoint_count == 0)
            throw std::runtime_error("vault restore without checkpoint");
        m_stack = checkpoint_stack[--checkpoint_count];
    }

    void clear() {
        clear_stack();  // wipe pointer stack        
        for (size_t i = 0; i < m_size; ++i) {
            buffer[i].~T();
        }
        m_size = 0;
        m_stack = 0;
        checkpoint_count = 0;
    }

    void lock() { locked = true; }
    void unlock() { locked = false; }
    bool is_locked() const { return locked; }

    void seal() { sealed = true; }
    bool is_sealed() const { return sealed; }
    
    // Additional helpers expected by tests
    size_t checkpoint_depth() const { return checkpoint_count; }
    void clear_checkpoints() { checkpoint_count = 0; }

    // Checkout removes the slot at index and returns the pointer.
    T* checkout(size_t index) {
        if (m_stack == 0) throw std::runtime_error("vault::checkout empty");
        if (index >= m_stack) throw std::out_of_range("vault::checkout out of range");
        T* p = m_slots[index];
        // shift left
        for (size_t i = index; i + 1 < m_stack; ++i) m_slots[i] = m_slots[i+1];
        --m_stack;
        return p;
    }

    // Return raw pointer to owned buffer
    T* data() { return buffer; }
    const T* data() const { return buffer; }

    // Simple bidirectional iterator over slot elements (dereferences to T&)
    struct iterator {
        vault* parent = nullptr;
        size_t idx = 0;
        iterator() = default;
        iterator(vault* p, size_t i) : parent(p), idx(i) {}
        T& operator*() const { return *parent->m_slots[idx]; }
        T* operator->() const { return parent->m_slots[idx]; }
        iterator& operator++() { ++idx; return *this; }
        iterator& operator--() { --idx; return *this; }
        bool operator==(const iterator& o) const { return parent == o.parent && idx == o.idx; }
        bool operator!=(const iterator& o) const { return !(*this == o); }
    };

    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, m_stack); }
};

#endif // VAULT_HPP
