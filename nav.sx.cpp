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

    // ignore transfers
    if ( to != get_self() || memo == get_self().to_string() || from == "token.sx"_n || from == "eosio.ram"_n) return;

    // must be *.sx account
    check( from.suffix() == "sx"_n, "must be *.sx account");

    // only accepts USDT
    check( get_first_receiver() == "tethertether"_n && quantity.symbol == symbol{"USDT", 4}, "only 4,USDT@tethertether is accepted");
    check( quantity >= asset{1000, symbol{"USDT", 4}}, "transfer must exceed 0.1000 USDT");

    // issue SX @ 1:1 ratio
    eosio::token::issue_action issue( "token.sx"_n, { "token.sx"_n, "active"_n });
    eosio::token::transfer_action transfer_token( "token.sx"_n, { "token.sx"_n, "active"_n });
    eosio::token::transfer_action transfer_self( "token.sx"_n, { get_self(), "active"_n });

    // issue 2x (1x => from, 1x => nav.sx)
    const asset out = asset{quantity.amount, symbol{"SX", 4}};
    issue.send( "token.sx"_n, out, "nav" );
    transfer_token.send( "token.sx"_n, get_self(), out, "nav" );
    transfer_self.send( get_self(), from, out, "nav" );
}
