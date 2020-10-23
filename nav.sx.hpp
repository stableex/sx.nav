#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

namespace sx {

using eosio::name;
using eosio::asset;
using eosio::symbol;
using eosio::string;

using eosio::string;

class [[eosio::contract("nav.sx")]] nav : public eosio::contract {
public:
    using contract::contract;

    /**
     * Notify contract when any token transfer notifiers relay contract
     */
    [[eosio::on_notify("*::transfer")]]
    void on_transfer( const name from, const name to, const asset quantity, const string memo );
};
}