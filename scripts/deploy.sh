#!/bin/bash

# unlock wallet
cleos wallet unlock --password $(cat ~/eosio-wallet/.pass)

# create accounts
cleos create account eosio sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio nav.sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio token.sx EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio myaccount EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio tethertether EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# deploy
cleos set contract nav.sx . nav.sx.wasm nav.sx.abi
cleos set contract token.sx . eosio.token.wasm eosio.token.abi
cleos set contract tethertether . eosio.token.wasm eosio.token.abi

# permission
cleos set account permission nav.sx active --add-code
cleos set account permission token.sx active nav.sx --add-code

# create SX
cleos push action token.sx create '["token.sx", "100000000.0000 SX"]' -p token.sx
cleos push action token.sx issue '["token.sx", "5000000.0000 SX", "init"]' -p token.sx
cleos transfer token.sx myaccount "50000.0000 SX" "init" --contract token.sx

# create USDT
cleos push action tethertether create '["tethertether", "100000000.0000 USDT"]' -p tethertether
cleos push action tethertether issue '["tethertether", "5000000.0000 USDT", "init"]' -p tethertether
cleos transfer tethertether nav.sx "50000.0000 USDT" "nav.sx" --contract tethertether
