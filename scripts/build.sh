#!/bin/bash

eosio-cpp nav.sx.cpp
cleos set contract nav.sx . nav.sx.wasm nav.sx.abi
