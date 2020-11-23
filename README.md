# `nav.sx`

> SX NAV (Net Asset Value)

## Quickstart

```bash
# `issue` USDT => SX @ 1:1 ratio
$ cleos transfer myaccount nav.sx "1.0000 USDT" "" --contract tethertether

# `redeem` SX => USDT @ 1:1 ratio
$ cleos transfer myaccount nav.sx "1.0000 SX" "" --contract token.sx
```

## Table of Content

- [STATIC `get_amount_out`](#static-get_amount_out)
- [TABLE `settings`](#table-settings)
- [TABLE `pairs`](#table-pairs)

## STATIC `get_amount_out`

Given an input amount of an asset and pair reserves, returns the maximum output amount of the other asset

### params

- `{name} contract` - contract name
- `{asset} amount_in` - amount input

### example

```c++
// Inputs
const name contract = "nav.sx"_n;
const asset amount_in = asset{10000, symbol{"USDT", 4}};

// Calculation
const asset amount_out = sx::nav::get_amount_out( contract, amount_in );
// => {"quantity": "1.0000 SX", "contract": "token.sx"}
```

## TABLE `settings`

- `{uint8_t} fee` - nav fee (pips 1/100 of 1%)

### example

```json
{
    "fee": 0
}
```

## TABLE `pairs`

- `{extended_symbol} base` - base asset
- `{extended_symbol} quote` - quote asset

### example

```json
{
    "base": {"symbol": "4,USDT", "contract": "tethertether"},
    "quote": {"symbol": "4,SX", "contract": "token.sx"}
}
```
