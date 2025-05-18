// -------------------------------------------------------------
//
// !\file test_vault.cpp
// !\brief Unit tests for the vault class
// !\author Colin J.D. Stewart
//
// -------------------------------------------------------------
//
//             Copyright (c) 2025. Colin J.D. Stewart
//                     All rights reserved
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/src/inc/shocktest.hpp"

// sut
#include "vault.hpp"

//
// !\brief Dummy class for testing
//
struct Dummy {
    int id;
    std::string label;

    Dummy(int i, const std::string& s) : id(i), label(s) {}
};

//
// !\brief Emplace and access elements in the vault
// !\details This test checks if we can emplace elements into the vault and access them correctly.
//
SHOCKTEST_GOODWEATHER(vault_emplace_and_access) {
    vault<Dummy> v;
    Dummy* d1 = v.emplace(1, "test");
    v.emplace(2, "hello");

    EXPECT_EQ(v.count(), 2);
    EXPECT_EQ(d1->id, 1);
    EXPECT_EQ(v[1].label, "hello");
}

//
// !\brief Restore trims back to the last checkpoint
//
SHOCKTEST_GOODWEATHER(vault_checkpoint_and_restore) {
    vault<Dummy> v;
    v.emplace(1, "a");
    v.checkpoint();
    v.emplace(2, "b");
    v.emplace(3, "c");

    EXPECT_EQ(v.count(), 3);
    v.restore();
    EXPECT_EQ(v.count(), 1);
    EXPECT_EQ(v[0].id, 1);
}

//
// !\brief Nested checkpoints restore in correct order
//
SHOCKTEST_GOODWEATHER(vault_multiple_checkpoints) {
    vault<Dummy> v;
    v.emplace(10, "base");
    v.checkpoint();
    v.emplace(20, "stage1");
    v.checkpoint();
    v.emplace(30, "stage2");

    v.restore();
    EXPECT_EQ(v.count(), 2);
    EXPECT_EQ(v[1].id, 20);

    v.restore();
    EXPECT_EQ(v.count(), 1);
    EXPECT_EQ(v[0].id, 10);
}

//
// !\brief Sealing and locking state management
//
SHOCKTEST_GOODWEATHER(vault_seal_and_lock) {
    vault<Dummy> v;
    v.emplace(42, "ok");
    v.seal();
    EXPECT_TRUE(v.is_sealed());
    EXPECT_THROW(v.emplace(99, "fail"));
    v.lock();
    EXPECT_TRUE(v.is_locked());
    v.unlock();
    EXPECT_FALSE(v.is_locked());
}

//
// !\brief Restore with no checkpoint should fail
//
SHOCKTEST_BADWEATHER(vault_restore_without_checkpoint_should_fail) {
    vault<Dummy> v;
    EXPECT_THROW(v.restore());
}

//
// !\brief Exceeding checkpoint stack should throw error
//
SHOCKTEST_BADWEATHER(vault_checkpoint_stack_overflow_should_fail) {
    vault<Dummy, 3> v;
    v.checkpoint();
    v.checkpoint();
    v.checkpoint();
    EXPECT_THROW(v.checkpoint());
}

//
// !\brief Clearing all checkpoints resets stack
//
SHOCKTEST_GOODWEATHER(vault_clear_checkpoints) {
    vault<Dummy> v;
    v.emplace(1, "a");
    v.checkpoint();
    v.emplace(2, "b");
    v.checkpoint();

    EXPECT_EQ(v.checkpoint_depth(), 2);
    v.clear_checkpoints();
    EXPECT_EQ(v.checkpoint_depth(), 0);
}

//
// !\brief Clear vault removes all entries
//
SHOCKTEST_GOODWEATHER(vault_clear_all) {
    vault<Dummy> v;
    v.emplace(1, "a");
    v.emplace(2, "b");
    v.clear();

    EXPECT_EQ(v.count(), 0);
    EXPECT_THROW(v[0]);
}

//
// !\brief Checkout and checkin do not alter vault state
//
SHOCKTEST_GOODWEATHER(vault_checkout_and_checkin) {
    vault<Dummy> v;
    v.emplace(1, "a");
    v.emplace(2, "b");

    Dummy* checked_out = v.checkout(0);
    EXPECT_EQ(checked_out->id, 1);

    v.checkin(checked_out);
    EXPECT_EQ(v.count(), 2);
}

//
// !\brief Checkout with invalid index should throw
//
SHOCKTEST_BADWEATHER(vault_checkout_invalid_index_should_fail) {
    vault<Dummy> v;
    v.emplace(1, "a");
    EXPECT_THROW(v.checkout(1));
}

//
// !\brief Checkout on empty vault should throw
//
SHOCKTEST_BADWEATHER(vault_checkout_on_empty_should_fail) {
    vault<Dummy> v;
    EXPECT_THROW(v.checkout(0));
}

//
// !\brief Top returns most recent element
//
SHOCKTEST_GOODWEATHER(vault_top) {
    vault<Dummy> v;
    v.emplace(1, "top");
    Dummy* top = v.top();
    EXPECT_EQ(top->id, 1);
}

//
// !\brief Top on empty vault should throw
//
SHOCKTEST_BADWEATHER(vault_top_on_empty_should_fail) {
    vault<Dummy> v;
    EXPECT_THROW(v.top());
}

//
// !\brief Get returns element by index
//
SHOCKTEST_GOODWEATHER(vault_get) {
    vault<Dummy> v;
    v.emplace(1, "a");
    Dummy* d = v.get(0);
    EXPECT_EQ(d->id, 1);
}

//
// !\brief Get with invalid index should throw
//
SHOCKTEST_BADWEATHER(vault_get_invalid_index_should_fail) {
    vault<Dummy> v;
    v.emplace(1, "a");
    EXPECT_THROW(v.get(1));
}

//
// !\brief Get on empty vault should throw
//
SHOCKTEST_BADWEATHER(vault_get_on_empty_should_fail) {
    vault<Dummy> v;
    EXPECT_THROW(v.get(0));
}

//
// !\brief Begin and end iterators access values in order
//
SHOCKTEST_GOODWEATHER(vault_begin_and_end) {
    vault<Dummy> v;
    v.emplace(1, "a");
    v.emplace(2, "b");

    auto it = v.begin();
    EXPECT_EQ(it->id, 1);
    ++it;
    EXPECT_EQ(it->id, 2);

    it = v.end();
    --it;
    EXPECT_EQ(it->id, 2);
}

//
// !\brief Operator[] gives access to elements by index
//
SHOCKTEST_GOODWEATHER(vault_operator_index) {
    vault<Dummy> v;
    v.emplace(1, "a");
    v.emplace(2, "b");

    EXPECT_EQ(v[0].id, 1);
    EXPECT_EQ(v[1].id, 2);
}

//
// !\brief Operator[] with invalid index throws
//
SHOCKTEST_BADWEATHER(vault_operator_index_invalid_index_should_fail) {
    vault<Dummy> v;
    v.emplace(1, "a");
    EXPECT_THROW(v[1]);
}

//
// !\brief Operator[] on empty vault throws
//
SHOCKTEST_BADWEATHER(vault_operator_index_on_empty_should_fail) {
    vault<Dummy> v;
    EXPECT_THROW(v[0]);
}

//
// !\brief Data returns raw pointer to internal buffer
//
SHOCKTEST_GOODWEATHER(vault_data) {
    vault<Dummy> v;
    v.emplace(1, "a");
    v.emplace(2, "b");

    Dummy* data = v.data();
    EXPECT_EQ(data[0].id, 1);
    EXPECT_EQ(data[1].id, 2);
}
