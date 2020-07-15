#include "eosio.token.hpp"
#include "nav.sx.hpp"

[[eosio::on_notify("*::transfer")]]
void navSx::on_transfer( const name from, const name to, const asset quantity, const string memo )
{
    // authenticate incoming `from` account
    require_auth( from );

    // ignore EOSIO system accounts
    const set<name> ignore = set<name>{ "eosio.stake"_n, "eosio.ram"_n, "eosio.rex"_n };
    if ( ignore.find( from ) != ignore.end() ) return;

    // ignore transfers
    if ( to != get_self() || memo == get_self().to_string() || from == "token.sx"_n ) return;

    // add docs
    docs _docs( get_self(), get_self().value );
    if ( !_docs.exists() ) _docs.get_or_create( get_self() );

    // user input validation
    const name contract = get_first_receiver();
    const symbol_code symcode = quantity.symbol.code();
    check(symcode == symbol_code{"SX"} || symcode == symbol_code{"USDT"}, "only accepts SX or USDT token transfers");
    check(contract == "token.sx"_n || contract == "tethertether"_n, "only accepts `token.sx` or `tethertether` contracts");

    // handle incoming SX
    if ( symcode == symbol_code{"SX"} ) {
        check( contract == "token.sx"_n, "invalid SX token contract" );
        check( memo == "USDT", "`memo` must be \"USDT\"" );
        burn( quantity );
        const asset out = asset{ quantity.amount, symbol{"USDT", 4}};
        self_transfer( from, extended_asset{ out, "tethertether"_n }, "nav");

    // handle incoming USDT
    } else if ( symcode == symbol_code{"USDT"} ) {
        check( contract == "tethertether"_n, "invalid USDT token contract" );
        check( memo == "SX", "`memo` must be \"SX\"" );
        const asset out = asset{ quantity.amount, symbol{"SX", 4}};
        issue( out );
        self_transfer( from, extended_asset{ out, "token.sx"_n }, "nav");
    }
}

void navSx::self_transfer( const name to, const extended_asset ext_quantity, const string memo )
{
    eosio::token::transfer_action transfer( ext_quantity.contract, { get_self(), "active"_n });
    transfer.send( get_self(), to, ext_quantity.quantity, memo );
}

// transfer & retire SX tokens
void navSx::burn( const asset quantity )
{
    eosio::token::transfer_action transfer( "token.sx"_n, { get_self(), "active"_n });
    transfer.send( get_self(), "token.sx"_n, quantity, "nav" );

    eosio::token::retire_action retire( "token.sx"_n, { "token.sx"_n, "active"_n });
    retire.send( quantity, "nav" );
}

// issue & transfer SX tokens
void navSx::issue( const asset quantity )
{
    eosio::token::issue_action issue( "token.sx"_n, { "token.sx"_n, "active"_n });
    issue.send( "token.sx"_n, quantity, "nav" );

    eosio::token::transfer_action transfer( "token.sx"_n, { "token.sx"_n, "active"_n });
    transfer.send( "token.sx"_n, get_self(), quantity, "nav" );
}
