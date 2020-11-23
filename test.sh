#!/bin/bash

# settings
cleos push action nav.sx setsettings '[[0]]' -p sx -p nav.sx

# pairs
cleos push action nav.sx setpair '[["4,USDT", "tethertether"], ["4,SX", "token.sx"]]' -p sx -p nav.sx