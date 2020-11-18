# `nav.sx`

> SX NAV (Net Asset Value)

## Quickstart

```bash
# `issue` USDT => SX @ 1:1 ratio
$ cleos transfer myaccount nav.sx "1.0000 USDT" "" --contract tethertether

# `redeem` SX => USDT @ 1:1 ratio + 0.1% fee
$ cleos transfer myaccount nav.sx "1.0000 SX" "" --contract token.sx
```
