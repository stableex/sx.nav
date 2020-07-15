# `nav.sx`

> SX NAV (Net Asset Value)

## Quickstart

USDT <=> SX

```bash
$ cleos transfer myaccount nav.sx "1.0000 USDT" "SX" --contract tethertether
$ cleos transfer myaccount nav.sx "1.0000 SX" "USDT" --contract token.sx
```

## Build

```
$ eosio-cpp nav.sx.cpp
```