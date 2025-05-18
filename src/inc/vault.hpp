// -------------------------------------------------------------
//
// !\file vault.hpp
// !\brief Vault class for managing a dynamic array of objects
// !\details This class provides a growable array with emplace
// !\author Colin J.D. Stewart
//
// -------------------------------------------------------------
//
//             Copyright (c) 2025. OverHertz Ltd.
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
    size_t size = 0;
    size_t capacity = 0;

    bool sealed = false;
    bool locked = false;

    // checkpoint stack
    size_t checkpoint_stack[MaxCheckpoints];
    size_t checkpoint_count = 0;

public:
    vault(size_t initial = 16) {
        reserve(initial);
    }

    ~vault() {
        clear();
        if (buffer) std::free(buffer);
    }

    // disable copy
    vault(const vault&) = delete;
    vault& operator=(const vault&) = delete;

    // allow move
    vault(vault&& other) noexcept {
        buffer = other.buffer;
        size = other.size;
        capacity = other.capacity;
        sealed = other.sealed;
        locked = other.locked;
        checkpoint_count = other.checkpoint_count;

        for (size_t i = 0; i < checkpoint_count; ++i) {
            checkpoint_stack[i] = other.checkpoint_stack[i];
        }

        other.buffer = nullptr;
        other.size = 0;
        other.capacity = 0;
        other.checkpoint_count = 0;
    }

    vault& operator=(vault&& other) noexcept {
        if (this != &other) {
            clear();
            if (buffer) std::free(buffer);

            buffer = other.buffer;
            size = other.size;
            capacity = other.capacity;
            sealed = other.sealed;
            locked = other.locked;
            checkpoint_count = other.checkpoint_count;

            for (size_t i = 0; i < checkpoint_count; ++i) {
                checkpoint_stack[i] = other.checkpoint_stack[i];
            }

            other.buffer = nullptr;
            other.size = 0;
            other.capacity = 0;
            other.checkpoint_count = 0;
        }
        return *this;
    }

    // growable reserve
    void reserve(size_t newCap) {
        if (sealed) throw std::runtime_error("vault is sealed; cannot reserve more capacity.");
        if (newCap <= capacity) return;

        T* newBuffer = static_cast<T*>(std::malloc(newCap * sizeof(T)));
        if (!newBuffer) throw std::bad_alloc();

        // move or copy elements into new buffer
        for (size_t i = 0; i < size; ++i) {
            new (newBuffer + i) T(std::move(buffer[i]));
            buffer[i].~T();
        }

        std::free(buffer);
        buffer = newBuffer;
        capacity = newCap;
    }


    // emplace new object, return pointer to it
    template<typename... Args>
    T* emplace(Args&&... args) {
        if (sealed) throw std::runtime_error("vault is sealed; cannot emplace.");
        if (size >= capacity) reserve(capacity ? capacity * 2 : 16);

        T* ptr = &buffer[size];
        new (ptr) T(std::forward<Args>(args)...);
        ++size;
        return ptr;
    }

    // get pointer at index
    T* get(size_t index) {
        if (index >= size) throw std::out_of_range("vault::get out of range");
        return &buffer[index];
    }

    const T* get(size_t index) const {
        if (index >= size) throw std::out_of_range("vault::get const out of range");
        return &buffer[index];
    }

    // accessors
    T& operator[](size_t index) { return *get(index); }
    const T& operator[](size_t index) const { return *get(index); }

    T* begin() { return buffer; }
    T* end() { return buffer + size; }
    const T* begin() const { return buffer; }
    const T* end() const { return buffer + size; }

    // size and data
    size_t count() const { return size; }
    T* data() { return buffer; }
    const T* data() const { return buffer; }

    // stack-like helpers
    T* top() {
        if (size == 0) throw std::runtime_error("vault::top called on empty vault");
        return &buffer[size - 1];
    }

    T* checkout(size_t index) { return get(index); }
    void checkin(T* /*ptr*/) { /* no-op for now */ }

    // checkpoint logic
    void checkpoint() {
        if (checkpoint_count >= MaxCheckpoints)
            throw std::runtime_error("vault checkpoint stack overflow");

        checkpoint_stack[checkpoint_count++] = size;
    }

    void restore() {
        if (checkpoint_count == 0)
            throw std::runtime_error("vault restore with no checkpoint");

        size_t target = checkpoint_stack[--checkpoint_count];

        for (size_t i = target; i < size; ++i) {
            buffer[i].~T();
        }

        size = target;
    }

    size_t checkpoint_depth() const { return checkpoint_count; }
    void clear_checkpoints() { checkpoint_count = 0; }

    // clear all values
    void clear() {
        for (size_t i = 0; i < size; ++i) {
            buffer[i].~T();
        }
        size = 0;
        checkpoint_count = 0;
    }

    // lock/seal
    void lock() { locked = true; }
    void unlock() { locked = false; }
    bool is_locked() const { return locked; }

    void seal() { sealed = true; }
    bool is_sealed() const { return sealed; }
};

#endif // VAULT_HPP
// -------------------------------------------------------------
// end of file