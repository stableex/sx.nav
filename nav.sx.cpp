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
    if ( to != get_self() || memo == get_self().to_string() || from == "eosio.ram"_n) return;

    // only accept USDT
    eosio::check( get_first_receiver() == "tethertether"_n && quantity.symbol == symbol{"USDT", 4}, "only 4,USDT@tethertether is accepted");

    // issue SX @ 1:1 ratio
    eosio::token::issue_action issue( "token.sx"_n, { "token.sx"_n, "active"_n });
    eosio::token::transfer_action transfer( "token.sx"_n, { get_self(), "active"_n });

    // issue 2x (1x => from, 1x => nav.sx)
    issue.send( "token.sx"_n, quantity * 2, "nav" );
    transfer.send( "token.sx"_n, get_self(), quantity * 2, "nav" );
    transfer.send( get_self(), from, quantity, "nav" );
}
