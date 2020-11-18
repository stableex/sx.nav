#include <eosio.token/eosio.token.hpp>

#include "nav.sx.hpp"

/**
 * Notify contract when any token transfer notifiers relay contract
 */
[[eosio::on_notify("*::transfer")]]
void sx::nav::on_transfer( const name from, const name to, const asset quantity, const string memo )
{
    // authenticate incoming `from` account
    require_auth( from );

    // accepted tokens
    const extended_symbol base = {{"SX", 4}, "token.sx"_n};
    const extended_symbol quote = {{"USDT", 4}, "tethertether"_n};

    // ignore transfers
    if ( to != get_self() || memo == get_self().to_string() || from == base.contract || from == "eosio.ram"_n) return;

    // must be *.sx account
    check( from.suffix() == "sx"_n, "must be *.sx account");

    // check incoming transfer
    const name contract = get_first_receiver();
    const extended_symbol incoming = extended_symbol{quantity.symbol, contract};
    check( base == incoming || quote == incoming, "only accepts `USDT` or `SX` tokens");
    check( quantity.amount >= 1000, "minimum transfer amount of 1000");

    // redeem SX => USDT @ 1:1 ratio + 0.1% fee
    if ( incoming == base ) {
        const int64_t fee = quantity.amount * 10 / 10000;
        retire( { quantity, contract }, "retire" );
        transfer( get_self(), from, { quantity.amount - fee, quote }, "nav" );

    // issue USDT => SX @ 1:1 ratio
    } else {
        const extended_asset out = { quantity.amount, base };
        issue( out, "issue" );
        transfer( get_self(), from, out, "nav" );
    }
}

void sx::nav::issue( const extended_asset value, const string memo )
{
    eosio::token::issue_action issue( value.contract, { value.contract, "active"_n });
    issue.send( value.contract, value.quantity, memo );
    transfer( value.contract, get_self(), value, memo );
}

void sx::nav::retire( const extended_asset value, const string memo )
{
    transfer( get_self(), value.contract, value, memo );
    eosio::token::retire_action retire( value.contract, { value.contract, "active"_n });
    retire.send( value.quantity, memo );
}

void sx::nav::transfer( const name from, const name to, const extended_asset value, const string memo )
{
    eosio::token::transfer_action transfer( value.contract, { from, "active"_n });
    transfer.send( from, to, value.quantity, memo );
}
